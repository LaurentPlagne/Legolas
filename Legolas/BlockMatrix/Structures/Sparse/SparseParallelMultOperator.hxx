#ifndef __LEGOLAS_SPARSEPARALLELMULTOPERATOR_HXX__
#define __LEGOLAS_SPARSEPARALLELMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualMultOperator.hxx"

namespace Legolas{
  
  class SparseParallelMultOperator : public SparseVirtualMultOperator{
  public:

    inline std::string name( void ) const { return "SparseParallelMultOperator" ;}
    
    void addMult(const SparseVirtualBlockMatrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y);
    void mult(const SparseVirtualBlockMatrix & A,const VirtualVector & X, VirtualVector & Y);
    
    SparseParallelMultOperator( void );
    ~SparseParallelMultOperator( void );
    SparseParallelMultOperator(const SparseParallelMultOperator & source) ; 

    virtual VirtualMultOperator * clone( void ) const {
      return new SparseParallelMultOperator(*this);
    }
    
  };
}

#endif
    
