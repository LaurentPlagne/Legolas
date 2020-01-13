#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalParallelSolver.hxx"
#include "my_tbb_parallel_for.hxx"

namespace Legolas{

  DiagonalParallelSolver::DiagonalParallelSolver( void ){}
  DiagonalParallelSolver::DiagonalParallelSolver(const DiagonalParallelSolver & source){}
  DiagonalParallelSolver::~DiagonalParallelSolver( void ){}

  class RangeSolverOperator{
    const DiagonalVirtualBlockMatrix & A_;
    const VirtualVector & B_;
    VirtualVector & X_;
  public:
    RangeSolverOperator(const DiagonalVirtualBlockMatrix & A, 
			const VirtualVector & B, 
			VirtualVector & X):A_(A),B_(B),X_(X){}
    
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      
      for (int i=r.begin() ; i!=r.end() ; i++){
	A_.diagonalGetElement(i).solve(B_.getElement(i),X_.getElement(i));
      }
    }
  };
  


  
  void DiagonalParallelSolver::solve(const DiagonalVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){
    
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),RangeSolverOperator(A,B,X));

    //    for (int i=A.beginRow() ; i < A.endRow() ; i++ ){
    //      A.diagonalGetElement(i).solve(B.getElement(i),X.getElement(i));
    //    }
    
  }
}
    
