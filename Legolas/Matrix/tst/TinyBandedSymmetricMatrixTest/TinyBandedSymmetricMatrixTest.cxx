/**
 * project  DESCARTES
 *
 * @file     TinyBandedSymmetricMatrixTest.cxx
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

  INFOS("A.nrows()="<<A.nrows());
  INFOS("A.ncols()="<<A.ncols());

  Legolas::displayLatex(A,"A.tex",0);



}
