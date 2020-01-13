#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedGaussSeidelSolver.hxx"

namespace Legolas{

  BandedGaussSeidelSolver::BandedGaussSeidelSolver( void ):accumulatorPtr_(0){}
  BandedGaussSeidelSolver::BandedGaussSeidelSolver(const BandedGaussSeidelSolver & source):accumulatorPtr_(0){}
  BandedGaussSeidelSolver::~BandedGaussSeidelSolver( void ){
    if (accumulatorPtr_){
      delete accumulatorPtr_;
      accumulatorPtr_=0;
    }
  }
  
  void BandedGaussSeidelSolver::solve(const BandedVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){

    this->iterationControler().initialize(A,X);
    
    do {

      const VirtualVector & B0=B.getElement(0);
      VirtualVector & accumulator=VirtualVector::getClone(accumulatorPtr_,B0);
      
      for (int i=A.beginRow() ; i < A.endRow() ; i++ ){
	
	accumulator.copy(B.getElement(i));

	for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	  if (i!=j) A.bandedGetElement(i,j).addMult(-1.0,X.getElement(j),accumulator);
	}
	
	A.bandedGetElement(i,i).solve(accumulator,X.getElement(i));
      }
	
    }while(!this->iterationControler().end(X));
  }

  
}
    
