#ifndef __LEGOLAS_SPARSEMULTOPERATOR_HXX__
#define __LEGOLAS_SPARSEMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualMultOperator.hxx"

namespace Legolas{
  
  class SparseMultOperator : public SparseVirtualMultOperator{
  public:

    inline std::string name( void ) const { return "SparseMultOperator" ;}
    
    void addMult(const SparseVirtualBlockMatrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y);
    void mult(const SparseVirtualBlockMatrix & A,const VirtualVector & X, VirtualVector & Y);

    void transposeAddMult(const SparseVirtualBlockMatrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y);
    void transposeMult(const SparseVirtualBlockMatrix & A,const VirtualVector & X, VirtualVector & Y);
    
    SparseMultOperator( void );
    ~SparseMultOperator( void );
    SparseMultOperator(const SparseMultOperator & source) ; 

    virtual VirtualMultOperator * clone( void ) const {
      return new SparseMultOperator(*this);
    }
    
  };
}

#endif
    
