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

# include "Legolas/Vector/Vector.hxx"

#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"

#include "Legolas/Algorithm/SparseGaussSeidel.hxx"
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"

#include "X86Timer.hxx"

#include "Legolas/Algorithm/ATLASDenseMatrixVectorProduct.hxx"

# include "Legolas/Matrix/MatrixVectorOperations.hxx"


using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");

  typedef double RealType;

  // Vectors X,B,R
  Legolas::Vector<double> X,B,R; 

  // don't change this size ! 
  const int size=3;

  X.resize(size);
  B.resize(size);
  R.resize(size);
  
  X=0.0;
  
  B[0]=18.0;
  B[1]=17.0;
  B[2]=17.0;

  R=0.0;

  // Matrix A

  typedef Actual StorageMode;

  typedef InputMatrixOptions<StorageMode,
                             SparseGaussSeidel,
                             ATLASDenseMatrixVectorProduct,
                             CVectorContainer,
                             DenseF77MatrixContainer,
                             Dense> AMatrixOptions;


  typedef GenericMatrixInterface<AMatrixDefinition<RealType>,AMatrixOptions,RealElementInterface<RealType> > AMatrixInterface;

  //  Legolas::MatrixShape<1> matrixShape(size,size);
  AMatrixInterface::Matrix A(Legolas::MatrixShape<1>(size,size));


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

  X=1.0;
  INFOS("X"<<X);  

  R=0.0;

  INFOS("R"<<X);  

  R=A*X;
  
  INFOS("A*X"<<R);  

  R=transpose(A)*X;
  
  INFOS("transpose(A)*X"<<R);  


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




}
