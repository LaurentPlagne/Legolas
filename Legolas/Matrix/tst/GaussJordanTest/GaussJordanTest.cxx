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

#include "Legolas/Algorithm/InverseDenseMatrixByGaussJordanAlgorithm.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  const int size=3;

  // ********* Matrix construction : A ***********
  typedef AMatrixDefinition<double>     MD ;
  typedef InputMatrixOptions<Actual>    MO ;
  typedef RealElementInterface<double>  EI ;

  typedef GenericMatrixInterface<MD,MO,EI> AMatrixInterface;

  Legolas::MatrixShape<1> ms(size,size);
  AMatrixInterface::Matrix A(ms);

  A(0,0)=5.0; A(0,1)=2.0; A(0,2)=3.0;
  A(1,0)=3.0; A(1,1)=4.0; A(1,2)=2.0;
  A(2,0)=1.0; A(2,1)=2.0; A(2,2)=4.0;

  A.display("A");

  // ********* Vector construction : X,B,R ***********
  
  Vector<double> X,B,R; 
  
  X.resize(size); B.resize(size); R.resize(size);
  
  X=0.0;   R=0.0;
  
  B[0]=18.0;
  B[1]=17.0;
  B[2]=17.0;

  cout << "B      "<< B << endl ;

  // ********* Linear solver test : AX=B ***********

  Legolas::solve(A,X,B); // solves AX=B
  cout << "Legolas::solve(A,X,B)  "<< X << endl ;
  
  R=A*X;
  cout << "R=A*X  "<< R << endl ;

  R-=B;
  cout << "R=A*X-B"<< R << endl ;


  AMatrixInterface::Matrix Am1(A);
  InverseDenseMatrixByGaussJordanAlgorithm::apply(Am1);

  Am1.display("A");

  X=Am1*B;
  cout << "X=Am1*BA  "<< X << endl ;
  R=A*X;
  cout << "R=A*X  "<< R << endl ;
  R-=B;
  cout << "R=A*X-B"<< R << endl ;

  



}
