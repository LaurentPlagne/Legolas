/**
 * project  DESCARTES
 *
 * @file     SparseMatrixTest.cxx
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

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "Legolas/Vector/Vector.hxx"

# include "ConstSparseMatrixDefinition.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");

  typedef double RealType;

  // don't change this size ! 
  const int size=4;

  // Vectors X,B,R
  Legolas::Vector<RealType> X(size),B(size),R(size);

  B[0]=0.814179;
  B[1]=1.089563;
  B[2]=1.11942;
  B[3]=1.58127;

  // Matrix A
  
  typedef Legolas::Virtual StorageMode;
  
  typedef SparseMatrixDefinition< RealType > SMD;
  typedef Legolas::InputMatrixOptions<StorageMode>    SMO;
  typedef Legolas::RealElementInterface<RealType>     MEI;
  
  typedef Legolas::GenericMatrixInterface<SMD,SMO,MEI> AMatrixInterface;
  
  Legolas::MatrixShape<1> ms(4,4);

  AMatrixInterface::Matrix A(ms);

  Legolas::displayLatex(A,"A.tex",0);
  Legolas::displayPNG(A,"A.png",2);

  INFOS("A.getMaxIteration()="<<A.getMaxIteration());
  INFOS("A.getEpsilon()="<<A.getEpsilon());
  
  Legolas::solve(A,X,B);

  INFOS("X"<<X);
  
  R=A*X;

  INFOS("A*X"<<R);

  R-=B;

  INFOS("A*X-B"<<R);

  Legolas::transposeSolve(A,X,B);
  //  X=B/transpose(A);

  INFOS("X"<<X);
  
  R=transpose(A)*X;

  INFOS("transpose(A)*X"<<R);

  R-=B;

  INFOS("transpose(A)*X-B"<<R);



}
