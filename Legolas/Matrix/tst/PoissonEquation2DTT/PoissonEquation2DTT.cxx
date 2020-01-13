/**
 * project  DESCARTES
 *
 * @file     DiagonalizationMatrixTest.cxx
 *
 * @author Laurent PLAGNE
 * @date   september 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
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
#include "Legolas/Algorithm/SparseGaussSeidel.hxx"

#include "SourceFunctor.hxx"

using namespace std; 
using namespace Legolas;

int main( int argc,  char *argv[] )
{
  typedef Legolas::Vector<double> Vector1D;

  
  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  
  const SizeType meshSize=200;
  
  Vector1D mesh(meshSize);
  //  mesh.resize(meshSize);
  double stepSize=1.0/double(meshSize-1);
  for (SizeType i = 0 ; i < mesh.size() ; i++) mesh[i]=double(i)*stepSize;
  
  double pi=2.0*acos(0.0); const double omega=4.0*pi;

  SourceFunctor<double> myFunction(omega);

  // ********* Vector construction : U,S,R ***********

  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions
  const SizeType size=meshSize-2;

  typedef Legolas::MultiVector<double,2> Vector2D;

  Vector2D::Shape shape(size,size);
    
  Vector2D U(shape); 
  
 //  U.resize(size); 
//   for (SizeType i=0 ; i < size ; i++){
//     U[i].resize(size);
//   }
  
  Vector2D S(U),R(U);
  
  for (SizeType i=0 ; i < size ; i++){
    for (SizeType j=0 ; j < size ; j++){
      const double x=mesh[i+1];
      const double y=mesh[j+1];

      S[i][j]=myFunction(x,y)*stepSize*stepSize;
    }
  }

  // Boundary conditions 
  
  // j=0 et j=N
  for (SizeType i=0 ; i < size ; i++){
    const double x=mesh[i+1];
    const double ui0=myFunction.exact(x,mesh[0]);
    const double uin=myFunction.exact(x,mesh[mesh.size()-1]);
    S[i][0]-=ui0;
    S[i][size-1]-=uin;
  }

  for (SizeType j=0 ; j < size ; j++){
    const double y=mesh[j+1];
    const double u0j=myFunction.exact(mesh[0],y);
    const double unj=myFunction.exact(mesh[mesh.size()-1],y);
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

  
  if (size<100){


  chronos.start();

  // MXMY * B

  XP=0.0;

  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      for (SizeType k=0 ; k <size ; k++){
	for (SizeType l=0 ; l <size ; l++){
	  
	  XP[i][j]+=M(i,k)*M(j,l)*S[k][l];
	}
      }
    }
  }

  // * D-1

  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      XP[i][j]/=D.diagonalGetElement(i)+D.diagonalGetElement(j);
    }
  }
  

  U=0.0;
  // Mxm1MyM& * 

  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      for (SizeType k=0 ; k <size ; k++){
	for (SizeType l=0 ; l <size ; l++){
	  
	  U[i][j]+=Mm1(i,k)*Mm1(j,l)*XP[k][l];
	}
      }
    }
  }  

  chronos.stop();
  //  chronos.storeResult("Tensorial v1");

  }



  // MxMy * S

  chronos.start();

  XPP=0.0;

  for (SizeType k=0 ; k <size ; k++){
    for (SizeType j=0 ; j <size ; j++){
      for (SizeType l=0 ; l <size ; l++){
	XPP[k][j]+=M(j,l)*S[k][l];
      }
    }
  }
  
  XP=0.0;
  
  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      for (SizeType k=0 ; k <size ; k++){
	XP[i][j]+=M(i,k)*XPP[k][j];
      }
    }
  }

  // * DXY^-1 XP

  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      XP[i][j]/=D.diagonalGetElement(i)+D.diagonalGetElement(j);
    }
  }

  // Mxm1Mym1 * XP

  XPP=0.0;

  for (SizeType k=0 ; k <size ; k++){
    for (SizeType j=0 ; j <size ; j++){
      for (SizeType l=0 ; l <size ; l++){
	XPP[k][j]+=Mm1(j,l)*XP[k][l];
      }
    }
  }
  
  U=0.0;
  
  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      for (SizeType k=0 ; k <size ; k++){
	U[i][j]+=Mm1(i,k)*XPP[k][j];
      }
    }
  }

  chronos.stop();
  //  chronos.storeResult("Tensorial v2");

  {

    double timeinSecond=chronos.get_elapsed_time_in_second();
    double nbops=8.0*double(size)*double(size)*double(size);
    double mflops=nbops/(timeinSecond*1.e6);
    cout << "mflops="<<mflops<<endl;

  }


  chronos.start();

  for (SizeType k=0 ; k <size ; k++){
    for (SizeType j=0 ; j <size ; j++){
      double tmp=0.0;
      for (SizeType l=0 ; l <size ; l++){
	tmp+=M(j,l)*S[k][l];
      }
      XPP[k][j]=tmp;
    }
  }
  
  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      double tmp=0.0;
      for (SizeType k=0 ; k <size ; k++){
	tmp+=M(i,k)*XPP[k][j];
      }
      XP[i][j]=tmp;
    }
  }

  // * D-1

  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      XP[i][j]/=D.diagonalGetElement(i)+D.diagonalGetElement(j);
    }
  }
  


  // Mxm1MyM& * 

  for (SizeType k=0 ; k <size ; k++){
    for (SizeType j=0 ; j <size ; j++){
      double tmp=0.0;
      for (SizeType l=0 ; l <size ; l++){
	tmp+=Mm1(j,l)*XP[k][l];
      }
      XPP[k][j]=tmp;
    }
  }
  
  for (SizeType i=0 ; i <size ; i++){
    for (SizeType j=0 ; j <size ; j++){
      double tmp=0.0;
      for (SizeType k=0 ; k <size ; k++){
	tmp+=Mm1(i,k)*XPP[k][j];
      }
      U[i][j]=tmp;
    }
  }

  chronos.stop();
  //  chronos.storeResult("Tensorial v3");

  {
    double timeinSecond=chronos.get_elapsed_time_in_second();
    double nbops=8.0*double(size)*double(size)*double(size);
    double mflops=nbops/(timeinSecond*1.e6);
    cout << "mflops="<<mflops<<endl;
  }
  


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


  {

    double timeinSecond=chronos.get_elapsed_time_in_second();
    double nbops=8.0*double(size)*double(size)*double(size);
    double mflops=nbops/(timeinSecond*1.e6);
    cout << "mflops="<<mflops<<endl;
  }

  ofstream outfile ("result.dat",ios::out) ;

  for (SizeType i=1 ; i < mesh.size()-1 ; i++){
    for (SizeType j=1 ; j < mesh.size()-1 ; j++){

      double xi= mesh[i];
      double yj= mesh[j];
      double uij=U[i-1][j-1];
      double eij=myFunction.exact(xi,yj);
      
      outfile << xi << " " << yj << " " << uij << " " << eij << " " << uij-eij<< " " << endl;
    }
  }

}
