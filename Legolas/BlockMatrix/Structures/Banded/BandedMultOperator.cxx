#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedMultOperator.hxx"

namespace Legolas{

  BandedMultOperator::BandedMultOperator( void ){}
  BandedMultOperator::BandedMultOperator(const BandedMultOperator & source){}
  BandedMultOperator::~BandedMultOperator( void ){}
  
  void BandedMultOperator::addMult(const BandedVirtualBlockMatrix & A, const double & a,
				   const VirtualVector & X, VirtualVector & Y){
    
    for (int i=0 ; i< A.nrows() ; i++){
      for (int j=std::max(i-A.linf(),0);j<=std::min(i+A.lsup(),A.nrows()-1);j++){
	A.bandedGetElement(i,j).addMult(a,X.getElement(j),Y.getElement(i));
      }
    }
    
  }

  void BandedMultOperator::mult(const BandedVirtualBlockMatrix & A,
				const VirtualVector & X, VirtualVector & Y){
    
    for (int i=0 ; i< A.nrows() ; i++){
      //first col
      int j=std::max(i-A.linf(),0);
      if (j<=std::min(i+A.lsup(),A.nrows()-1)){//This line is not empty
	A.sparseGetElement(i,j).mult(X.getElement(j),Y.getElement(i));
      }
      else{//Empty line
	Y.getElement(i).scale(0.0);
      }
      j=A.nextColInRow(i,j);
      //remaining cols
      for (;j<=std::min(i+A.lsup(),A.nrows()-1);j++){
	A.bandedGetElement(i,j).addMult(1.0,X.getElement(j),Y.getElement(i));
      }
    }
    
  }
}
    
