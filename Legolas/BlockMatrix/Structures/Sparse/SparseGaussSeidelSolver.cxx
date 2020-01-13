#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseGaussSeidelSolver.hxx"
#include <set>

namespace Legolas{

  SparseGaussSeidelSolver::SparseGaussSeidelSolver( void ):accumulatorPtr_(0){}
  SparseGaussSeidelSolver::SparseGaussSeidelSolver(const SparseGaussSeidelSolver & source):accumulatorPtr_(){
  }
  SparseGaussSeidelSolver::~SparseGaussSeidelSolver( void ){
    if (accumulatorPtr_){
      delete accumulatorPtr_;
      accumulatorPtr_=0;
    }
  }
  
  void SparseGaussSeidelSolver::solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){

    //    INFOS("Entering SparseGaussSeidelSolver::solve for the matrix :"<<A.name());

    this->iterationControler().initialize(A,X);

    const VirtualVector & B0=B.getElement(0);
    VirtualVector & accumulator=VirtualVector::getClone(accumulatorPtr_,B0);
    
    //    INFOS("ICI"); B.display(); 
    //    INFOS("ICI"); accumulator.display(); 

    do {

      for (int i=A.beginRow() ; i < A.endRow() ; i++ ){

	//	INFOS("i="<<i);
	
	accumulator.copy(B.getElement(i));

	//	INFOS("accumulator"); accumulator.display();
	
	for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	  if (i!=j){
	    //	    INFOS("j="<<j);
	    A.sparseGetElement(i,j).addMult(-1.0,X.getElement(j),accumulator);
	  }
	}

	//	INFOS("accumulator"); accumulator.display();

	
	A.sparseGetElement(i,i).solve(accumulator,X.getElement(i));

	

      }
	

    }while(!this->iterationControler().end(X));
  }
  
  
  void SparseGaussSeidelSolver::transposeSolve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){

    //    INFOS("ICI");
    const VirtualVector & B0=B.getElement(0);
    VirtualVector & accumulator=VirtualVector::getClone(accumulatorPtr_,B0);


    std::vector< std::set<int> > transposeNonZeroElements(A.ncols());
    for (int i=A.endRow()-1 ; i >= A.beginRow() ; i-- ){
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	transposeNonZeroElements[j].insert(i);
      }
    }

    do {

      for (int i=transposeNonZeroElements.size()-1 ; i>=0 ; i--){

	accumulator.copy(B.getElement(i));

	auto iter=transposeNonZeroElements[i].begin();
	for ( ; iter!=transposeNonZeroElements[i].end(); iter++ ){
	  const int j=*iter;
	  if (i!=j){
	    A.sparseGetElement(j,i).transposeAddMult(-1.0,X.getElement(j),accumulator);
	  }
	}

	A.sparseGetElement(i,i).transposeSolve(accumulator,X.getElement(i));
      
      }
    
    }while(!this->iterationControler().end(X));
  }
  
}
    
