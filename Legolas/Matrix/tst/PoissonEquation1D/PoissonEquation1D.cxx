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

#include "LaplacianMatrixDefinition.hxx"


#include "X86Timer.hxx"


using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  //  typedef Legolas::Vector<double> Vector1D;
  typedef Legolas::MultiVector<double,1> Vector1D;

  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  const SizeType meshSize=100;
  
  Vector1D mesh;
  mesh.resize(meshSize);
  double stepSize=1.0/double(meshSize-1);
  for (SizeType i = 0 ; i < mesh.size() ; i++) mesh[i]=double(i)*stepSize;

  // the source term is equal to -omega2*sin(omega*x);
  
  double pi=2.0*acos(0.0); 
  const double omega=5.0*pi;
  const double omega2=omega*omega;
  
  // ********* Vector construction : U,B,R ***********

  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions
  const SizeType size=meshSize-2;

  Vector1D U,B,R; 
  
  U.resize(size);   B.resize(size);   R.resize(size); 
  U=0.0;        
  R=0.0;
  
  for (SizeType i=0 ; i < B.size() ; i++){
    const double x=mesh[i+1];
    B[i]=-1.0*omega2*sin(omega*x)*stepSize*stepSize;
  }

  // Boundary conditions : B[0]=delta*rho-U[0] where U[O]=sin(omega*mesh[0]);
  
  const double u0=sin(omega*mesh[0]);
  const double un=sin(omega*mesh[mesh.size()-1]);

  B[0]-=u0;
  B[size-1]-=un;

  
  // ********* Matrix construction : LaplacianMatrixInterface ***********
  
  typedef LaplacianMatrixDefinition                 LMD;
  typedef InputMatrixOptions<>                      LMO;
  // typedef InputMatrixOptions<Virtual,TriDiagonalLU>  LMO;
  typedef RealElementInterface<double>              LEI ;
  typedef GenericMatrixInterface<LMD,LMO,LEI>       LMatrixInterface;

  Legolas::MatrixShape<1> ms(size,size);
  LMatrixInterface::Matrix A(ms);

  // ********* Linear solver test : AU=B ***********

  X86Timer chronos; 
  
  chronos.start();
  Legolas::solve(A,U,B);
  //  U=B/A; // solves AU=B
  chronos.stop();

  chronos.storeResult("U=B/A");

  R=A*U;

  R-=B;

  cout << "Residual="<< (Legolas::dot(R,R)) << endl ;

  ofstream outfile ("result.dat",ios::out) ;
  
  for (SizeType i=1 ; i < mesh.size()-1 ; i++){

    double xi=mesh[i];
    double ui=U[i-1];
    double ei=sin(omega*xi);

    outfile << xi  << " " << ui << " " << ei << " " << ui - ei<< " " << endl;
  }
  
  outfile.close();

}
