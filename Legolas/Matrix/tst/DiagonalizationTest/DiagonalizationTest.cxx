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
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "DenseMatrixDefinition.hxx"
# include "DiagonalMatrixDefinition.hxx"
# include "LaplacianMatrixDefinition.hxx"

#include "Legolas/Algorithm/TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm.hxx"
#include "Legolas/Algorithm/DiagonalMatrixInversion.hxx"
#include "Legolas/Algorithm/TriDiagonalMatrixInversionByLUDecomposition.hxx"
#include "X86Timer.hxx"
#include "Legolas/Algorithm/ATLASDenseMatrixVectorProduct.hxx"
#include "Legolas/Algorithm/SparseGaussSeidel.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  X86Timer chronos;

  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  
  const SizeType meshSize=100;
  const SizeType nbCell=meshSize-1;
  
  Vector<double> mesh;
  mesh.resize(meshSize);
  double stepSize=1.0/double(nbCell);
  for (SizeType i = 0 ; i < mesh.size() ; i++) mesh[i]=double(i)*stepSize;
  
  // the source term is equal to -omega2*sin(omega*x);
  
  double pi=acos(0.0); 
  const double omega=5.0*pi;
  const double omega2=omega*omega;
  
  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions

  const SizeType size=meshSize-2;

  typedef InputMatrixOptions<Actual>    MO ;
  // ********* Matrix construction : A ***********
  
  typedef LaplacianMatrixDefinition     AMD ;
  typedef InputMatrixOptions<Virtual,TriDiagonalMatrixInversionByLUDecomposition> AMO;
  typedef RealElementInterface<double>  EI ;

  typedef GenericMatrixInterface<AMD,AMO,EI> AMatrixInterface;
  
  Legolas::MatrixShape<1> ms(size,size);
  AMatrixInterface::Matrix A(ms);

  //  A.display("A");

  // ********* Matrix construction : M ***********

  typedef DenseMatrixDefinition     MMD ;
  typedef InputMatrixOptions<Actual,SparseGaussSeidel,ATLASDenseMatrixVectorProduct,CVectorContainer,DenseF77MatrixContainer,Dense> DenseMO;
  typedef GenericMatrixInterface<MMD,DenseMO,EI> MMatrixInterface;
  
  MMatrixInterface::Matrix M(ms);
  MMatrixInterface::Matrix Mm1(ms);

  //  M.display("M");
  //  Mm1.display("Mm1");

  // ********* Matrix construction : D ***********
  
  typedef DiagonalMatrixDefinition     DMD ;
  typedef InputMatrixOptions<Actual,DiagonalMatrixInversion> DMO;
  typedef GenericMatrixInterface<DMD,DMO,EI> DMatrixInterface;
  
  DMatrixInterface::Matrix D(ms);
  
  //  D.display("D");

  // ********* Vector construction : X,B,R ***********
  Vector<double> X,B,R,XP,XPP; 
  
  X.resize(size); B.resize(size); R.resize(size); XP.resize(size); XPP.resize(size);
  
  
  for (SizeType i=0 ; i < size ; i++)  B[i]=-1.0*omega2*sin(omega*mesh[i+1])*stepSize*stepSize;

  // Boundary conditions : B[0]=delta*rho-U[0] where U[O]=sin(omega*mesh[0]);
  
  double u0=sin(omega*mesh[0]);
  double un=sin(omega*mesh[mesh.size()-1]);

  B[0]-=u0;
  B[size-1]-=un;

  //

  X=0.0;   R=0.0; 
  
  cout << "B      "<< B << endl ;

  // ********* Linear solver test : AX=B ***********
  

  chronos.start();
  Legolas::solve(A,X,B); // solves AX=B
  chronos.stop();

  chronos.storeResult("Legolas::solve(A,X,B)");


  cout << "Legolas::solve(A,X,B)  "<< X << endl ;
  
  R=A*X;
  cout << "R=A*X  "<< R << endl ;

  R-=B;
  cout << "R=A*X-B"<< R << endl ;

  // ********* Diagonalization by QL algo : A=Mm1 * D * M ***********

  chronos.start();
  TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm::apply(A,M,Mm1,D);
  chronos.stop();
  chronos.storeResult("QLAlgo");

  //  M.display("M");
  //  Mm1.display("Mm1");
  //  D.display("D");
  
  chronos.start();
  XP=M*B;
  chronos.stop();
  chronos.storeResult("X=M*B");
  chronos.start();
  XPP=XP/D;
  chronos.stop();
  chronos.storeResult("X=XP/DB");
  chronos.start();
  X=Mm1*XPP;
  chronos.stop();
  chronos.storeResult("X=Mm1*XPP");

  cout << "X=MDm1Mm1*B"<< X << endl ;


  ofstream outfile ("result.dat",ios::out) ;

  outfile << mesh[0] << " " << u0 << " " << sin(omega*mesh[0]) << " " << 0.0 <<endl;
  for (SizeType i=1 ; i < mesh.size()-1 ; i++){
    outfile << mesh[i] << " " << X[i-1] << " " << sin(omega*mesh[i]) << " " << X[i-1]-sin(omega*mesh[i]) << " " << endl;
  }
  outfile << mesh[mesh.size()-1] << " " << un << " " << sin(omega*mesh[mesh.size()-1]) << " " << 0.0<<endl;

  outfile.close();


}
