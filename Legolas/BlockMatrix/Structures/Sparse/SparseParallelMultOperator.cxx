#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseParallelMultOperator.hxx"

#include "my_tbb_parallel_for.hxx"

namespace Legolas{

  SparseParallelMultOperator::SparseParallelMultOperator( void ){}
  SparseParallelMultOperator::SparseParallelMultOperator(const SparseParallelMultOperator & source){}
  SparseParallelMultOperator::~SparseParallelMultOperator( void ){}
  
  ////////////////////////////////////////////////////////////////////////////////
  class AddMultLineOperator{
    const SparseVirtualBlockMatrix & A_;
    const double & a_;
    const VirtualVector & X_; 
    VirtualVector & Y_;
  public:
    AddMultLineOperator(const SparseVirtualBlockMatrix & A,
			const double & a,
			const VirtualVector & X, VirtualVector & Y):A_(A),a_(a),X_(X),Y_(Y){}
    
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      
      for (int i=r.begin() ; i!=r.end() ; i++){
	for (int j=A_.beginColInRow(i) ; j < A_.endColInRow(i) ; j=A_.nextColInRow(i,j) ){
	  A_.sparseGetElement(i,j).addMult(a_,X_.getElement(j),Y_.getElement(i));
	}
      }
    }
  };
  
  void SparseParallelMultOperator::addMult(const SparseVirtualBlockMatrix & A, const double & a,
					   const VirtualVector & X, VirtualVector & Y){
    
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),AddMultLineOperator(A,a,X,Y));
    
    // for (int i=0 ; i< A.nrows() ; i++){
    //   for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
    // 	A.sparseGetElement(i,j).addMult(a,X.getElement(j),Y.getElement(i));}
    // }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  class MultLineOperator{
    const SparseVirtualBlockMatrix & A_;
    const VirtualVector & X_; 
    VirtualVector & Y_;
  public:
    MultLineOperator(const SparseVirtualBlockMatrix & A,
		     const VirtualVector & X, VirtualVector & Y):A_(A),X_(X),Y_(Y){}
    
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      
      for (int i=r.begin() ; i!=r.end() ; i++){
	//first col
	int j=A_.beginColInRow(i);
	if (j<A_.endColInRow(i)){//This line is not empty
	  A_.sparseGetElement(i,j).mult(X_.getElement(j),Y_.getElement(i));
	}
	else{//Empty line
	  Y_.getElement(i).scale(0.0);
	}
	j=A_.nextColInRow(i,j);
	
	//remaining cols
	for ( ; j < A_.endColInRow(i) ; j=A_.nextColInRow(i,j) ){
	  A_.sparseGetElement(i,j).addMult(1.0,X_.getElement(j),Y_.getElement(i));
	}
	//	for (int j=A_.beginColInRow(i) ; j < A_.endColInRow(i) ; j=A_.nextColInRow(i,j) ){
	//	  A_.sparseGetElement(i,j).addMult(a_,X_.getElement(j),Y_.getElement(i));
	//	}
      }
    }
  };
  
  void SparseParallelMultOperator::mult(const SparseVirtualBlockMatrix & A,
					const VirtualVector & X, VirtualVector & Y){
    
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),MultLineOperator(A,X,Y));
    
    // for (int i=0 ; i< A.nrows() ; i++){
    //   for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
    // 	A.sparseGetElement(i,j).addMult(a,X.getElement(j),Y.getElement(i));}
    // }
    
  }
}
    
