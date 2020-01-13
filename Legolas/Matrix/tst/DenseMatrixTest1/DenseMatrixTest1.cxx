/**
 * project  DESCARTES
 *
 * @file     DenseMatrixTest.cxx
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

# include "AMatrixDefinition.hxx"

#include <assert.h>

using namespace std; using namespace Legolas;
using namespace Legolas;

int main( int argc,  char *argv[] )
{
  const int size=3;

  // ********* Matrix construction : A ***********
  typedef AMatrixDefinition             MD ;
  typedef InputMatrixOptions<Actual>    MO ;
  typedef RealElementInterface<double>  EI ;

  typedef GenericMatrixInterface<MD,MO,EI> AMatrixInterface;

  Legolas::MatrixShape<1> ms(size,size);

  INFOS("ms="<<ms);

  AMatrixInterface::Matrix A(ms);

  A(0,0)=5.0; A(0,1)=2.0; A(0,2)=3.0;
  A(1,0)=3.0; A(1,1)=4.0; A(1,2)=2.0;
  A(2,0)=1.0; A(2,1)=2.0; A(2,2)=4.0;

  Legolas::displayLatex(A,"A.tex");

  //  A.display("A");

  // ********* Vector construction : X,B,R ***********

  Legolas::Vector<double> X(size),B(size),R(size);
  
  X=0.0;   R=0.0;
  
  B[0]=18.0;
  B[1]=17.0;
  B[2]=17.0;

  cout << "B      "<< B << endl ;

  // ********* Linear solver test : AX=B ***********
  
  Legolas::solve(A,X,B);
  //  A.matrixVectorInverse(B,X);

  //  AMatrixInterface::Matrix::Solver linearSolver(A,X,B);

  //  linearSolver.solve(A,X,B);

  //  Legolas::linearSolve(A,X,B);
  //  A.linearSole

  //  Legolas::solve(A,X,B); // solves AX=B
  cout << "Legolas::solve(A,X,B)  "<< X << endl ;
  
  R=A*X;
  cout << "R=A*X  "<< R << endl ;

  R-=B;
  cout << "R=A*X-B"<< R << endl ;

  double residual=Legolas::dot(R,R);
  INFOS("residual="<<residual);
  assert(residual<1.e-15);
}
