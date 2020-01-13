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

#include "Tensorial3DProduct.hxx"
#include "Tensorial3DProductTBB.hxx"
#include "SourceFunctor.hxx"
#include "UniformMesh.hxx"

using namespace std; 
using namespace Legolas;

int main( int argc,  char *argv[] )
{
  typedef float Real;


  typedef Legolas::Vector<Real> Vector1D;

  
  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  
  const SizeType meshSizeX=258;
  const SizeType meshSizeY=258;
  const SizeType meshSizeZ=258;
  
  Vector1D meshX=UniformMesh<Real>::build(meshSizeX);
  Vector1D meshY=UniformMesh<Real>::build(meshSizeY);
  Vector1D meshZ=UniformMesh<Real>::build(meshSizeZ);
  
  Real pi=2.0*acos(0.0); 
  
  const Real omegaX=2.0*pi;
  const Real omegaY=4.0*pi;
  const Real omegaZ=8.0*pi;

  SourceFunctor<Real> myFunction(omegaX,omegaY,omegaZ);

  // ********* Vector construction : U,S,R ***********

  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions
  const SizeType sizeX=meshSizeX-2;
  const SizeType sizeY=meshSizeY-2;
  const SizeType sizeZ=meshSizeZ-2;
  
  typedef Legolas::MultiVector<Real,2> Vector2D;
  typedef Legolas::MultiVector<Real,3> Vector3D;

  Vector2D::Shape shape2D(sizeY,sizeZ);
  Vector3D::Shape shape3D(sizeX,shape2D);

  Vector2D::Shape shape2Dxy(sizeX,sizeY);
  Vector3D::Shape shape3Dzxy(sizeZ,shape2Dxy);
    
  Vector3D Uzxy(shape3Dzxy); 
  
  INFOS("shape3Dzxy"<<shape3Dzxy);

  INFOS("Uzxy.size()="<<Uzxy.size());
  INFOS("Uzxy[0].size()="<<Uzxy[0].size());
  INFOS("Uzxy[0][0].size()="<<Uzxy[0][0].size());


  Vector3D U(shape3D); 
  Vector3D Up(shape3D); 
  
  Vector3D S(U),R(U);
  
  // Boundary conditions

  for (SizeType i=0 ; i < sizeX ; i++){
    for (SizeType j=0 ; j < sizeY ; j++){
      for (SizeType k=0 ; k < sizeZ ; k++){
	const Real x=meshX[i+1];
	const Real y=meshY[j+1];
	const Real z=meshZ[k+1];

	S[i][j][k]=myFunction(x,y,z);
      }
    }
  }

  //Dirichlet

  for (SizeType j=0 ; j < sizeY ; j++){
    for (SizeType k=0 ; k < sizeZ ; k++){
      { //Face x==0
	const Real deltaX=meshX[1]-meshX[0];
	const Real factor=1.0/(deltaX*deltaX);
	S[0][j][k]-=factor*myFunction.exact(meshX[0],meshY[j+1],meshZ[k+1]);
      }
      {
	//Face x==n
	const Real deltaX=meshX[meshX.size()-1]-meshX[meshX.size()-2];
	const Real factor=1.0/(deltaX*deltaX);
	S[sizeX-1][j][k]-=factor*myFunction.exact(meshX[meshX.size()-1],meshY[j+1],meshZ[k+1]);
      }
    }
  }
  
  for (SizeType i=0 ; i < sizeX ; i++){
    for (SizeType k=0 ; k < sizeZ ; k++){
      { //Face y==0
	const Real deltaY=meshY[1]-meshY[0];
	const Real factor=1.0/(deltaY*deltaY);
	S[i][0][k]-=factor*myFunction.exact(meshX[i+1],meshY[0],meshZ[k+1]);
      }
      
      {   //Face y==N
	const Real deltaY=meshY[meshY.size()-1]-meshY[meshY.size()-2];
	const Real factor=1.0/(deltaY*deltaY);
	S[i][sizeY-1][k]-=factor*myFunction.exact(meshX[i+1],meshY[meshY.size()-1],meshZ[k+1]);
      }
    }
  }

  for (SizeType i=0 ; i < sizeX ; i++){
    for (SizeType j=0 ; j < sizeY ; j++){
      { //Face z==0
	const Real deltaZ=meshZ[1]-meshZ[0];
	const Real factor=1.0/(deltaZ*deltaZ);
	S[i][j][0]-=factor*myFunction.exact(meshX[i+1],meshY[j+1],meshZ[0]);
      }
      
      {   //Face z==N
	const Real deltaZ=meshZ[meshZ.size()-1]-meshZ[meshZ.size()-2];
	const Real factor=1.0/(deltaZ*deltaZ);
	S[i][j][sizeZ-1]-=factor*myFunction.exact(meshX[i+1],meshY[j+1],meshZ[meshZ.size()-1]);
      }
    }
  }
  

  // ********* Matrix construction : LX,Y,Z ***********
  
  typedef LaplacianMatrixDefinition<Real>     AMD ;
  typedef InputMatrixOptions<Legolas::Virtual>   AMO;
  typedef RealElementInterface<Real>  EI ;

  typedef GenericMatrixInterface<AMD,AMO,EI> AMatrixInterface;
  

  AMatrixInterface::Matrix LX(meshX);
  AMatrixInterface::Matrix LY(meshY);
  AMatrixInterface::Matrix LZ(meshZ);

  //  AMatrixInterface::Matrix LX(LaplacianMatrixDefinition<Real>::Data(Legolas::MatrixShape<1>(sizeX,sizeX)));
  //  AMatrixInterface::Matrix LY(LaplacianMatrixDefinition<Real>::Data(Legolas::MatrixShape<1>(sizeY,sizeY)));
  //  AMatrixInterface::Matrix LZ(LaplacianMatrixDefinition<Real>::Data(Legolas::MatrixShape<1>(sizeZ,sizeZ)));
  
  // ********* Matrix construction : M,X,Y,Z ***********

  typedef DenseMatrixDefinition<Real>                  MMD ;
  typedef InputMatrixOptions<Actual,SparseGaussSeidel,ATLASDenseMatrixVectorProduct,CVectorContainer,DenseF77MatrixContainer,Dense> DenseMO;
  typedef GenericMatrixInterface<MMD,DenseMO,EI> MMatrixInterface;
  
  MMatrixInterface::Matrix MX( Legolas::MatrixShape<1>(sizeX,sizeX) );
  MMatrixInterface::Matrix MY( Legolas::MatrixShape<1>(sizeY,sizeY) );
  MMatrixInterface::Matrix MZ( Legolas::MatrixShape<1>(sizeZ,sizeZ) );

  MMatrixInterface::Matrix MXm1( Legolas::MatrixShape<1>(sizeX,sizeX) );
  MMatrixInterface::Matrix MYm1( Legolas::MatrixShape<1>(sizeY,sizeY) );
  MMatrixInterface::Matrix MZm1( Legolas::MatrixShape<1>(sizeZ,sizeZ) );

  {
    // Test mat mat 
    MMatrixInterface::Matrix A( Legolas::MatrixShape<1>(2,2) );
    MMatrixInterface::Matrix B( Legolas::MatrixShape<1>(2,2) );
    MMatrixInterface::Matrix C( Legolas::MatrixShape<1>(2,2) );

    A(0,0)=1.0 ;     A(0,1)=2.0 ; 
    A(1,0)=3.0 ;     A(1,1)=4.0 ; 

    B(0,0)=1.0 ;     B(0,1)=1.0 ; 
    B(1,0)=5.0 ;     B(1,1)=3.0 ; 

    A.display();
    B.display();

    Legolas::ATLASDenseMatrixMatrixProduct::apply(A,B,C);

    C.display();
  }

  {
    // Test mat mat 
    MMatrixInterface::Matrix A( Legolas::MatrixShape<1>(2,3) );
    MMatrixInterface::Matrix B( Legolas::MatrixShape<1>(3,4) );
    MMatrixInterface::Matrix C( Legolas::MatrixShape<1>(2,4) );

    A(0,0)=1.0 ; A(0,1)=2.0 ; A(0,2)=3.0 ; 
    A(1,0)=4.0 ; A(1,1)=5.0 ; A(1,2)=6.0 ; 

    B(0,0)=8.0 ; B(0,1)=7.0 ; B(0,2)=7.0 ; B(0,3)=5.0 ; 
    B(1,0)=4.0 ; B(1,1)=3.0 ; B(1,2)=2.0 ; B(1,3)=1.0 ; 
    B(2,0)=4.0 ; B(2,1)=3.0 ; B(2,2)=2.0 ; B(2,3)=1.0 ; 

    A.display();
    B.display();

    Legolas::ATLASDenseMatrixMatrixProduct::apply(A,B,C);

    C.display();
  }




  // ********* Matrix construction : D ***********
  
  typedef DiagonalMatrixDefinition<Real>     DMD ;
  typedef InputMatrixOptions<Actual>         DMO;
  typedef GenericMatrixInterface<DMD,DMO,EI> DMatrixInterface;
  
  DMatrixInterface::Matrix DX( Legolas::MatrixShape<1>(sizeX,sizeX) );
  DMatrixInterface::Matrix DY( Legolas::MatrixShape<1>(sizeY,sizeY) );
  DMatrixInterface::Matrix DZ( Legolas::MatrixShape<1>(sizeZ,sizeZ) );
  


  // ********* Diagonalization by QL algo : Ld=Mdm1 * Dd * Md ***********

  INFOS("Entering QL OK");

  X86Timer chronos;
  
  chronos.start();
  TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm::apply(LX,MX,MXm1,DX);
  TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm::apply(LY,MY,MYm1,DY);
  TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm::apply(LZ,MZ,MZm1,DZ);
  chronos.stop();
  INFOS("QL Algo time (s) :"<<chronos.get_elapsed_time_in_second());

  INFOS("QL OK");

  X86Timer chronos_solve;

  typedef Tensorial3DProductV0 TPS;
  //typedef Tensorial3DProductTBB TPS;

  chronos.start();
  chronos_solve.start();
  //  TPS::apply(MX,MY,MZ,S,Uzxy,Up);
  TPS::applyXYZ_ZXY(MX,MY,MZ,S,U,Uzxy);
  chronos.stop();

  {
    Real timeinSecond=chronos.get_elapsed_time_in_second();

    Real nbops=0.0+
      2.0*Real(sizeX)*Real(sizeX)*Real(sizeY)*Real(sizeZ)+
      2.0*Real(sizeX)*Real(sizeY)*Real(sizeY)*Real(sizeZ)+
      2.0*Real(sizeX)*Real(sizeY)*Real(sizeZ)*Real(sizeZ);
    Real mflops=nbops/(timeinSecond*1.e6);
    INFOS("mflops="<<mflops);
  }
  
  chronos.start();
  for (SizeType k=0 ; k< sizeZ ; k++){
    for (SizeType i=0 ; i< sizeX ; i++){
      for (SizeType j=0 ; j< sizeY ; j++){
	//	Up[i][j][k]/=(DX.diagonalGetElement(i)+DY.diagonalGetElement(j)+DZ.diagonalGetElement(k));
	Uzxy[k][i][j]/=(DX.diagonalGetElement(i)+DY.diagonalGetElement(j)+DZ.diagonalGetElement(k));
      }
    }
  }
  chronos.stop();
  Real timeinSecond=chronos.get_elapsed_time_in_second();
  INFOS("inverse time : timeinSecond:"<<timeinSecond);
  
  
  chronos.start();
  //  TPS::apply(MXm1,MYm1,MZm1,Up,Uzxy,U);
  TPS::applyZXY_XYZ(MXm1,MYm1,MZm1,Uzxy,U);
  chronos.stop();
  {
    Real timeinSecond=chronos.get_elapsed_time_in_second();
    Real nbops=0.0+
      2.0*Real(sizeX)*Real(sizeX)*Real(sizeY)*Real(sizeZ)+
      2.0*Real(sizeX)*Real(sizeY)*Real(sizeY)*Real(sizeZ)+
      2.0*Real(sizeX)*Real(sizeY)*Real(sizeZ)*Real(sizeZ);
    Real mflops=nbops/(timeinSecond*1.e6);
    INFOS("mflops="<<mflops);
  } 

  chronos_solve.stop();
  INFOS("Time for solving (s) : "<<chronos_solve.get_elapsed_time_in_second());

  ofstream outfile ("result.dat",ios::out) ;

  Real residual=0.0;
  Real normalize=0.0;

  for (SizeType i=1 ; i < meshX.size()-1 ; i++){
    for (SizeType j=1 ; j < meshY.size()-1 ; j++){
      for (SizeType k=1 ; k < meshZ.size()-1 ; k++){

	Real xi= meshX[i];
	Real yj= meshY[j];
	Real zj= meshZ[k];

	Real uij=U[i-1][j-1][k-1];
	Real eij=myFunction.exact(xi,yj,zj);
	
	Real diff=eij-uij;
	
	residual+=diff*diff;
	normalize+=eij*eij;
	if ( (k==(meshZ.size()/2) && (meshX.size()<200)))
	  outfile << xi << " " << yj << " " << uij << " " << eij << " " << uij-eij<< " " << endl;
      }
    }
  }
  residual/=normalize;

  INFOS("residual="<<std::sqrt(residual));
  
}
