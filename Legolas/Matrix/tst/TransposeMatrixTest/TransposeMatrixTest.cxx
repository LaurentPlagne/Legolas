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

# include "AMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "Legolas/Vector/Vector.hxx"

#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"
#include "Legolas/Algorithm/SparseGaussSeidel.hxx"
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"

#include "X86Timer.hxx"

template <class MATRIX>
double sum(const MATRIX & A){
  double s=0.0;
  for (int i=0 ; i < A.nrows() ; i++){
    for (int j=0 ; j < A.ncols() ; j++){
      s+=A(i,j);
    }
  }
  return s;
}



using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");

  typedef double RealType;

  // don't change this size ! 
  const int size=3;
  
  // Vectors X,B,R
  Legolas::Vector<double> X(size),B(size),R(size);

  B[0]=18.0;
  B[1]=17.0;
  B[2]=17.0;

  // Matrix A

  typedef Actual StorageMode;

  typedef InputMatrixOptions<StorageMode,
                             SparseGaussSeidel,
                             SparseMatrixVectorProduct,
                             STLVectorContainer,
                             DenseF77MatrixContainer,
                             Dense> AMatrixOptions;



  typedef GenericMatrixInterface<AMatrixDefinition<RealType>,AMatrixOptions,RealElementInterface<RealType> > AMatrixInterface;
  AMatrixDefinition<RealType>::Data init(size);
  AMatrixInterface::Matrix A( init );
  //  AMatrixInterface::Matrix A( size );


  A(0,0)=5.0;
  A(0,1)=2.0;
  A(0,2)=3.0;

  A(1,0)=3.0;
  A(1,1)=4.0;
  A(1,2)=2.0;

  A(2,0)=1.0;
  A(2,1)=2.0;
  A(2,2)=4.0;

  
  A.display("A");

  INFOS("A.getMaxIteration()="<<A.getMaxIteration());

  A.setMaxIteration()=100;
  INFOS("A.setMaxIteration()="<<A.setMaxIteration());


  INFOS("A.getEpsilon()="<<A.getEpsilon());

  A.setEpsilon()=1.e-10;
  INFOS("A.getEpsilon()="<<A.getEpsilon());

  Legolas::solve(A,X,B); 

  INFOS("X"<<X);
  
  R=A*X;

  INFOS("A*X"<<R);

  R-=B;

  INFOS("A*X-B"<<R);

  //  AMatrixInterface::TransposedView At(A);

  //  At.display("At");

  //transpose(A)*X;

  R=transpose(A)*X;

  INFOS("transpose(A)*X"<<R);

  R+=transpose(A)*X;

  INFOS("transpose(A)*X"<<R);

  R-=transpose(A)*X;

  INFOS("transpose(A)*X"<<R);

  ////////////////////////////////////////

  //  X=B/transpose(A);
  Legolas::transposeSolve(A,X,B); 



  INFOS("X=B/transpose(A)"<<X);
  
  R=transpose(A)*X;

  INFOS("transpose(A)*X"<<R);

  R-=B;

  INFOS("transpose(A)*X-B"<<R);

}
