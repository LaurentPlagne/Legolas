/**
 * project  DESCARTES
 *
 * @file     StationaryHeatEquationTest.cxx
 *
 * @author Laurent PLAGNE
 * @date   september 2010
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D
 */
# include <cstdlib>
# include <cmath>
# include <iostream>

# include "UTILITES.hxx" 
# include "Legolas/Vector/Vector.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixShape.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

#include "Legolas/Algorithm/TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm.hxx"
#include "Legolas/Algorithm/DiagonalMatrixVectorProduct.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/DRGenericMatrixInterface.hxx"


# include "DenseMatrixDefinition.hxx"
# include "DiagonalMatrixDefinition.hxx"
# include "LaplacianMatrixDefinition.hxx"

#include "X86Timer.hxx"
#include "Legolas/Algorithm/ATLASDenseMatrixVectorProduct.hxx"
#include "Legolas/Algorithm/ATLASDenseMatrixMatrixProduct.hxx"

#include "SourceFunctor.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  typedef Legolas::Vector<double> Vector1D;

  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  
  const SizeType meshSize=1024;
  
  Vector1D mesh(meshSize);
  //  mesh.resize(meshSize);
  double stepSize=1.0/double(meshSize-1);
  for (SizeType i = 0 ; i < mesh.size() ; i++) mesh[i]=double(i)*stepSize;
  
  double pi=2.0*acos(0.0); const double omega=4.0*pi;

  SourceFunctor<double> myFunction;

  // ********* Vector construction : U,S,R ***********
  
  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions
  const SizeType size=meshSize-2;

  typedef Legolas::MultiVector<double,2> Vector2D;

  Vector2D::Shape shape(size,size);
    
  Vector2D U(shape); 
  Vector2D S(U),R(U);
  
  for (SizeType i=0 ; i < size ; i++){
    for (SizeType j=0 ; j < size ; j++){
      S[i][j]=0.0;
    }
  }

  // Boundary conditions 
  
  // j=0 et j=N
  for (SizeType i=0 ; i < size ; i++){
    const double x=mesh[i+1];
    const double ui0=myFunction.dirichletBoundaryForV(x,mesh[0]);
    const double uin=myFunction.dirichletBoundaryForV(x,mesh[mesh.size()-1]);
    S[i][0]-=ui0;
    S[i][size-1]-=uin;
  }

  for (SizeType j=0 ; j < size ; j++){
    const double y=mesh[j+1];
    const double u0j=myFunction.dirichletBoundaryForV(mesh[0],y);
    const double unj=myFunction.dirichletBoundaryForV(mesh[mesh.size()-1],y);
    S[0][j]-=u0j;
    S[size-1][j]-=unj;
  }


  // ********* Matrix construction : A ***********
  
  typedef LaplacianMatrixDefinition     AMD ;
  typedef InputMatrixOptions<Legolas::Virtual>   AMO;
  typedef RealElementInterface<double>  EI ;

  typedef GenericMatrixInterface<AMD,AMO,EI> AMatrixInterface;
  
  LaplacianMatrixDefinition::Data data(Legolas::MatrixShape<1>(size,size));

  AMatrixInterface::Matrix L( data );

  // ********* Matrix construction : M ***********

  typedef DenseMatrixDefinition                  MMD ;
  //typedef InputMatrixOptions<Actual>             DenseMO;
  typedef InputMatrixOptions<Actual,SparseGaussSeidel,ATLASDenseMatrixVectorProduct,CVectorContainer,DenseF77MatrixContainer,Dense> DenseMO;
  typedef GenericMatrixInterface<MMD,DenseMO,EI> MMatrixInterface;
  
  MMatrixInterface::Matrix M( Legolas::MatrixShape<1>(size,size) );
  MMatrixInterface::Matrix Mm1( Legolas::MatrixShape<1>(size,size) );

  // ********* Matrix construction : D ***********
  
  typedef DiagonalMatrixDefinition           DMD ;
  typedef InputMatrixOptions<Actual>         DMO;
  typedef GenericMatrixInterface<DMD,DMO,EI> DMatrixInterface;
  
  DMatrixInterface::Matrix D( Legolas::MatrixShape<1>(size,size) );
  


  // ********* Diagonalization by QL algo : L=Mm1 * D * M ***********

  X86Timer chronos;
  
  chronos.start();
  TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm::apply(L,M,Mm1,D);
  chronos.stop();
  //  chronos.storeResult("QLAlgo");

    INFOS("QL OK");
  
  Vector2D XP(U);
  Vector2D XPP(U);
  
  // V4

  MMatrixInterface::Matrix MXP(Legolas::MatrixShape<1>(size,size));
  MMatrixInterface::Matrix MXPP(Legolas::MatrixShape<1>(size,size));
  MMatrixInterface::Matrix MU(Legolas::MatrixShape<1>(size,size));

  MMatrixInterface::Matrix MTS(Legolas::MatrixShape<1>(size,size));
  
  for (SizeType i=0 ; i< size ; i++){
    for (SizeType j=0 ; j< size ; j++){
      MTS(i,j)=S[j][i];
    }
  }

  chronos.start();

  ATLASDenseMatrixMatrixProduct::apply(M,MTS,MXPP);
  ATLASDenseMatrixMatrixProduct::apply(M,MXPP,MXP);

  // * D-1 + transpose...

  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      MXP(j,i)/=D.diagonalGetElement(i)+D.diagonalGetElement(j);
    }
  }
  
  ATLASDenseMatrixMatrixProduct::apply(Mm1,MXP,MXPP);
  ATLASDenseMatrixMatrixProduct::apply(Mm1,MXPP,MU);

  chronos.stop();
  //  chronos.storeResult("Tensorial v4");


  for (SizeType i=0 ; i< size ; i++){
    for (SizeType j=0 ; j< size ; j++){
      U[i][j]=MU(i,j);
    }
  }

  // Mxm1MyM& * 

  //  INFOS("U="<<U);



  {

    double timeinSecond=chronos.get_elapsed_time_in_second();
    double nbops=8.0*double(size)*double(size)*double(size);
    double mflops=nbops/(timeinSecond*1.e6);
    cout << "mflops="<<mflops<<endl;
  }

  {
    ofstream outfile ("inter.dat",ios::out) ;
    
    for (SizeType i=1 ; i < mesh.size()-1 ; i++){
      for (SizeType j=1 ; j < mesh.size()-1 ; j++){
	
	double xi= mesh[i];
	double yj= mesh[j];
	double uij=U[i-1][j-1];
	//      double eij=myFunction.exact(xi,yj);
	double eij=0.0;
      
	outfile << xi << " " << yj << " " << uij << " " << eij << " " << uij-eij<< " " << endl;
      }
    }
    
  outfile.close();
  }

  Vector2D Jx(shape),Jy(shape);
  Vector2D LJx(shape),LJy(shape);

  {
    // Fabrication de Jx=dV/dx
    double coef=0.5/stepSize;
    for (SizeType j=0 ; j< size ; j++){
      for (SizeType i=1 ; i< size-1 ; i++){
	Jx[i][j]=(U[i+1][j]-U[i-1][j])*coef;
      }
      Jx[0][j]=Jx[1][j];
      Jx[size-1][j]=Jx[size-2][j];
    }
  }
  

  {
    // Fabrication de Jy=dV/dy
    double coef=0.5/stepSize;
    for (SizeType i=0 ; i< size ; i++){
      for (SizeType j=1 ; j< size-1 ; j++){
	Jy[i][j]=(U[i][j+1]-U[i][j-1])*coef;
      }
      Jy[i][0]=Jy[i][1];
      Jy[i][size-1]=Jy[i][size-2];
    }
  }

  //  INFOS("Jy="<<Jy);

  // Fabrication de LJx=Jx/lambda
  // Fabrication de LJy=Jy/lambda
  for (SizeType i=0 ; i< size ; i++){
    for (SizeType j=0 ; j< size ; j++){
      double xi= mesh[i+1];
      double yj= mesh[j+1];
      
      double l=lambda(xi,yj);
      double lm1=1.0/l;
      
      LJx[i][j]=Jx[i][j]*lm1;
      LJy[i][j]=Jy[i][j]*lm1;
    }
  }

  
  double coef2=0.5*stepSize;

  // S=div(LJx)
  for (SizeType i=1 ; i< size-1 ; i++){
    for (SizeType j=1 ; j< size-1 ; j++){

      S[i][j]=coef2*(LJx[i+1][j  ]-LJx[i-1][j]+
		    LJy[i  ][j+1]-LJy[i  ][j-1]);
    }
  }
  //j=0
  for (SizeType i=1 ; i< size-1 ; i++){
    S[i][0]=coef2*(LJx[i+1][0]-LJx[i-1][0]);
  }
  //i=0
  for (SizeType j=1 ; j< size-1 ; j++){
    S[0][j]=coef2*(LJy[0][j+1]-LJy[0][j-1]);
  }

  S[0][0]=0.0;

  // j=0 et j=N
  for (SizeType i=0 ; i < size ; i++){
    const double x=mesh[i+1];
    const double ui0=myFunction.dirichletBoundaryForT(x,mesh[0]);
    const double uin=myFunction.dirichletBoundaryForT(x,mesh[mesh.size()-1]);
    S[i][0]-=ui0;
    S[i][size-1]-=uin;
  }

  for (SizeType j=0 ; j < size ; j++){
    const double y=mesh[j+1];
    const double u0j=myFunction.dirichletBoundaryForT(mesh[0],y);
    const double unj=myFunction.dirichletBoundaryForT(mesh[mesh.size()-1],y);
    S[0][j]-=u0j;
    S[size-1][j]-=unj;
  }

  
  for (SizeType i=0 ; i< size ; i++){
    for (SizeType j=0 ; j< size ; j++){
      MTS(i,j)=S[j][i];
    }
  }

  chronos.start();

  ATLASDenseMatrixMatrixProduct::apply(M,MTS,MXPP);
  ATLASDenseMatrixMatrixProduct::apply(M,MXPP,MXP);

  // * D-1 + transpose...

  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      MXP(j,i)/=D.diagonalGetElement(i)+D.diagonalGetElement(j);
    }
  }
  
  ATLASDenseMatrixMatrixProduct::apply(Mm1,MXP,MXPP);
  ATLASDenseMatrixMatrixProduct::apply(Mm1,MXPP,MU);

  chronos.stop();
  //  chronos.storeResult("Tensorial v4");


  for (SizeType i=0 ; i< size ; i++){
    for (SizeType j=0 ; j< size ; j++){
      U[i][j]=MU(i,j);
    }
  }

  // Mxm1MyM& * 


  {

    double timeinSecond=chronos.get_elapsed_time_in_second();
    double nbops=8.0*double(size)*double(size)*double(size);
    double mflops=nbops/(timeinSecond*1.e6);
    cout << "mflops="<<mflops<<endl;
  }

  {
  
    ofstream outfile ("result.dat",ios::out) ;
    
    for (SizeType i=1 ; i < mesh.size()-1 ; i++){
      for (SizeType j=1 ; j < mesh.size()-1 ; j++){
	
	double xi= mesh[i];
	double yj= mesh[j];
	double uij=U[i-1][j-1];
	//      double eij=myFunction.exact(xi,yj);
	double eij=0.0;
	
	outfile << xi << " " << yj << " " << uij << " " << eij << " " << uij-eij<< " " << endl;
      }
    }
  }
      
      
  
      




  



}
