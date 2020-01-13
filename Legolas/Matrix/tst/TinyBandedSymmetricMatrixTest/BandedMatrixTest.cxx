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

# include "Legolas/Vector/Vector.hxx"

#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");

  typedef double RealType;

  // Vectors X,B,R
  Legolas::Vector<RealType> X,B,R; 

  // don't change this size ! 
  const int size=5;

  X.resize(size);
  B.resize(size);
  R.resize(size);
  
  X=0.0;
  
  B[0]=85.0;
  B[1]=145.0;
  B[2]=213.0;
  B[3]=181.0;
  B[4]=150.0;

  R=0.0;

  // Matrix A

  typedef Actual StorageMode;


  typedef InputMatrixOptions<StorageMode> AMatrixOptions;

  typedef GenericMatrixInterface<AMatrixDefinition<RealType>,AMatrixOptions,RealElementInterface<RealType> > AMatrixInterface;


  Legolas::MatrixShape<1> ms(size,size);
  AMatrixInterface::Matrix A(ms);

  // diag

  A.bandedGetElement(0,0)=18.0;
  A.bandedGetElement(1,1)=19.0;
  A.bandedGetElement(2,2)=20.0;
  A.bandedGetElement(3,3)=21.0;
  A.bandedGetElement(4,4)=22.0;

  // j=i-1

  A.bandedGetElement(1,0)=7.0;
  A.bandedGetElement(2,1)=8.0;
  A.bandedGetElement(3,2)=9.0;
  A.bandedGetElement(4,3)=10.0;

  // j=i+1

  A.bandedGetElement(0,1)=11.0;
  A.bandedGetElement(1,2)=12.0;
  A.bandedGetElement(2,3)=13.0;
  A.bandedGetElement(3,4)=14.0;

  // j=i+2

  A.bandedGetElement(0,2)=15.0;
  A.bandedGetElement(1,3)=16.0;
  A.bandedGetElement(2,4)=17.0;

  //  A.display("A");

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
  

  // Transpose test

  B[0]=32.0;
  B[1]=73.0;
  B[2]=135.0;
  B[3]=205.0;
  B[4]=217.0;

  X=B/transpose(A);

  INFOS("X=B/transpose(A)"<<X);
  
  R=transpose(A)*X;

  INFOS("transpose(A)*X"<<R);

  R-=B;

  INFOS("transpose(A)*X-B"<<R);

}
