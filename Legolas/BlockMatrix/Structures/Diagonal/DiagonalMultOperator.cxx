#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalMultOperator.hxx"

namespace Legolas{

  DiagonalMultOperator::DiagonalMultOperator( void ){}
  DiagonalMultOperator::DiagonalMultOperator(const DiagonalMultOperator & source){}
  DiagonalMultOperator::~DiagonalMultOperator( void ){}
  
  void DiagonalMultOperator::addMult(const DiagonalVirtualBlockMatrix & A, const double & a,
				     const VirtualVector & X, VirtualVector & Y){
    
    for (int i=0 ; i< A.nrows() ; i++){
      A.diagonalGetElement(i).addMult(a,X.getElement(i),Y.getElement(i));
    }
    
  }


  void DiagonalMultOperator::mult(const DiagonalVirtualBlockMatrix & A,
				  const VirtualVector & X, VirtualVector & Y){
    
    for (int i=0 ; i< A.nrows() ; i++){
      A.diagonalGetElement(i).mult(X.getElement(i),Y.getElement(i));
    }
    
  }
}
    
