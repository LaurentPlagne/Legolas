#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalParallelMultOperator.hxx"
#include "my_tbb_parallel_for.hxx"


namespace Legolas{

  DiagonalParallelMultOperator::DiagonalParallelMultOperator( void ){}
  DiagonalParallelMultOperator::DiagonalParallelMultOperator(const DiagonalParallelMultOperator & source){}
  DiagonalParallelMultOperator::~DiagonalParallelMultOperator( void ){}

  class RangeDiagonalAddMultOperator{
    const DiagonalVirtualBlockMatrix & A_;
    const double & a_;
    const VirtualVector & X_;
    VirtualVector & Y_;
  public:
    RangeDiagonalAddMultOperator(const DiagonalVirtualBlockMatrix & A, const double & a,
				 const VirtualVector & X, VirtualVector & Y):A_(A),a_(a),X_(X),Y_(Y){}
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      for (int i=r.begin() ; i!=r.end() ; i++){
	A_.diagonalGetElement(i).addMult(a_,X_.getElement(i),Y_.getElement(i));
      }
    }
  };

  class RangeDiagonalMultOperator{
    const DiagonalVirtualBlockMatrix & A_;
    const VirtualVector & X_;
    VirtualVector & Y_;
  public:
    RangeDiagonalMultOperator(const DiagonalVirtualBlockMatrix & A,
				 const VirtualVector & X, VirtualVector & Y):A_(A),X_(X),Y_(Y){}
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      for (int i=r.begin() ; i!=r.end() ; i++){
	A_.diagonalGetElement(i).mult(X_.getElement(i),Y_.getElement(i));
      }
    }
  };
			      
      

  
  void DiagonalParallelMultOperator::addMult(const DiagonalVirtualBlockMatrix & A, const double & a,
					    const VirtualVector & X, VirtualVector & Y){
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),RangeDiagonalAddMultOperator(A,a,X,Y));
  }

  
  void DiagonalParallelMultOperator::mult(const DiagonalVirtualBlockMatrix & A,
					 const VirtualVector & X, VirtualVector & Y){
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),RangeDiagonalMultOperator(A,X,Y));
    
  }
}
    
