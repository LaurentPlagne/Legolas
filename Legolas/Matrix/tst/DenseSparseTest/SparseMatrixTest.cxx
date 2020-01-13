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

# include "SparseMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "Legolas/Vector/Vector.hxx"

#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"

#include "SparseMatrixDefinition2D.hxx"
#include "display.hxx"


double uniform()
{
  return static_cast<double>(std::rand())/static_cast<double>(RAND_MAX);
}


using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");

  typedef double RealType;

  const int size=30;

  // Matrix A

  typedef Actual StorageMode;
  
  typedef SparseMatrixDefinition< RealType > SMD;
  typedef InputMatrixOptions<StorageMode>    SMO;
  typedef RealElementInterface<RealType>     MEI;
  
  typedef GenericMatrixInterface<SMD,SMO,MEI> SparseMatrixInterface;
  
  SMD::Data init(size,size);

  ///////////////////////////////////////////////////:

  SparseMatrixInterface::Matrix A(init);

  INFOS("A.nrows()="<<A.nrows());
  INFOS("A.ncols()="<<A.ncols());

  for (int i=0 ; i <A.nrows() ; i++){
    for (int j=0 ; j <A.ncols() ; j++){
      //      if (uniform()*std::abs(i-j)<0.5) A(i,j)=1.0;
      if (uniform()*std::abs(i-j)<0.5) A.sparseGetElement(i,j)=1.0;
    }
  }
      

  Legolas::displayLatex(A,"A.tex",2);

  ///////////////////////////////////////////////////
    
    SparseMatrixInterface::Matrix D(SMD::Data(10,15));
    
    for (int i=0 ; i <D.nrows() ; i++){
      for (int j=0 ; j <D.ncols() ; j++){
	D.sparseGetElement(i,j)=1.0;
	//	D(i,j)=1.0;
      }
    }
      

    Legolas::displayLatex(D,"D.tex",2);

    

  
    
}
