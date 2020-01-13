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

#include "LaplacianMatrixDefinition.hxx"

#include "X86Timer.hxx"

#include "APVectorDefinition.hxx"
#include "TriDiagonalLU.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  static const int PRECISION=50; // not more than 300
  typedef GenericVector< APVectorDefinition<PRECISION> >::Vector Vector1D; 

  typedef Vector1D::RealType RealType;
  
  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  const int meshSize=100;

  Vector1D::RealType zero=0;
  Vector1D::RealType one=1;
  Vector1D::RealType minusOne=-1;
  Vector1D::RealType two=2;
  Vector1D::RealType five=5;

  Vector1D mesh;
  mesh.resize(meshSize);
  RealType stepSize=one/RealType(meshSize-1);
  for (int i = 0 ; i < mesh.size() ; i++) mesh[i]=RealType(i)*stepSize;

  // the source term is equal to -omega2*sin(omega*x);
  
  RealType myPi=pi(PRECISION);

  const RealType omega=five*myPi;
  const RealType omega2=omega*omega;
  
  // ********* Vector construction : U,B,R ***********

  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions
  const int size=meshSize-2;

  Vector1D U,B,R; 
  
  U.resize(size);   B.resize(size);   R.resize(size); 

  U=zero;        
  R=zero;
  
  for (int i=0 ; i < B.size() ; i++){
    const RealType x=mesh[i+1];
    B[i]=minusOne*omega2*sin(omega*x)*stepSize*stepSize;
  }

  // Boundary conditions : B[0]=delta*rho-U[0] where U[O]=sin(omega*mesh[0]);
  
  const RealType u0=sin(omega*mesh[0]);
  const RealType un=sin(omega*mesh[mesh.size()-1]);

  B[0]-=u0;
  B[size-1]-=un;

  cout << "B=" << B << endl;

  // ********* Matrix construction : LaplacianMatrixInterface ***********
  
  typedef LaplacianMatrixDefinition<PRECISION>      LMD;
  typedef InputMatrixOptions<Actual,TriDiagonalLU>  LMO;
  //  typedef RealElementInterface<RealType>              LEI ;
  typedef NoGenericVector< APDataDriver< PRECISION> > LEI;
  typedef GenericMatrixInterface<LMD,LMO,LEI>       LMatrixInterface;

  LMatrixInterface::Matrix A(size);

  // ********* Linear solver test : AU=B ***********

  X86Timer chronos; 
  
  chronos.start();
  U=B/A; // solves AU=B
  chronos.stop();

  chronos.storeResult("U=B/A");

  R=A*U;

  R-=B;

  cout << "Residual="<< (Vector1D::dot(R,R)) << endl ;

  ofstream outfile ("result.dat",ios::out) ;

  for (int i=1 ; i < mesh.size()-1 ; i++){

    RealType xi=mesh[i];
    RealType ui=U[i-1];
    RealType ei=sin(omega*xi);

    outfile << xi  << " " << ui << " " << ei << " " << ui - ei<< " " << endl;
  }
  
  outfile.close();

}
