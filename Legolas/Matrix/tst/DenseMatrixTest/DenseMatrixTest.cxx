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

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
#include "Legolas/Matrix/MatrixOptions/DefaultMatrixOptions.hxx"

//struct Toto{ typedef Legolas::DefaultMatrixOptions<double,double> Tutu;};

#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
#include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"
#include "Legolas/Matrix/MatrixVectorOperations.hxx"
#include "Legolas/Algorithm/SparseGaussSeidel.hxx"
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"

#include "AMatrixDefinition.hxx"
#include <assert.h>

using namespace std; using namespace Legolas;
using namespace Legolas;

int main( int argc,  char *argv[] )
{

 //  MESSAGE("debut du code");

  typedef double RealType;
  const int size=4;
  Legolas::Vector<RealType> X(size),B(size),R(size);
  
  X[0]=0.0;
  X[1]=0.0;
  X[2]=0.0;
  X[3]=0.0;
  
  B[0]=0.814179;
  B[1]=1.089563;
  B[2]=1.11942;
  B[3]=1.58127;
  
  //   // Matrix A

  typedef Legolas::Actual StorageMode;

  typedef InputMatrixOptions<StorageMode,
                             SparseGaussSeidel,
                             SparseMatrixVectorProduct,
                             STLVectorContainer,
                             DenseF77MatrixContainer,
                             Dense> AMatrixOptions;
  
  typedef AMatrixDefinition<RealType>    MD;
  typedef AMatrixOptions                 MO;
  typedef RealElementInterface<RealType> ME;

  typedef Legolas::GenericMatrixInterface<MD,MO,ME>  AMatrixInterface;

  Legolas::MatrixShape<1> ms(size,size);
  AMatrixInterface::Matrix A(ms);

  RealType a=11.1148;
  RealType b=-3.64756;
  RealType c=-3.81972;

  A(0,0)=a;
  A(0,1)=b;
  A(1,0)=b;
  A(1,1)=a;

  A(2,2)=a;
  A(2,3)=b;
  A(3,2)=b;
  A(3,3)=a;
  
  A(2,0)=c;
  A(3,1)=c;
  
  A.display("A");

  INFOS("A.getMaxIteration()="<<A.getMaxIteration());

  A.setMaxIteration()=100;
  INFOS("A.setMaxIteration()="<<A.setMaxIteration());


  INFOS("A.getEpsilon()="<<A.getEpsilon());

  A.setEpsilon()=1.e-10;
  INFOS("A.getEpsilon()="<<A.getEpsilon());

  INFOS("X"<<X);

  Legolas::solve(A,X,B);

  INFOS("X"<<X);
  
  R=A*X;

  INFOS("A*X"<<R);

  R-=B;

  INFOS("A*X-B"<<R);

  Legolas::displayLatex(A,"A.tex",0);

  assert(Legolas::dot(R,R)<1.e-20);


  //  X=B/transpose(A);
  Legolas::transposeSolve(A,X,B);

  INFOS("X"<<X);
  
  R=transpose(A)*X;

  INFOS("transpose(A)*X"<<R);

  R-=B;

  INFOS("transpose(A)*X-B"<<R);

  //  Legolas::displayLatex(transpose(A),"At.tex",0);  

 
  return 0;
}
