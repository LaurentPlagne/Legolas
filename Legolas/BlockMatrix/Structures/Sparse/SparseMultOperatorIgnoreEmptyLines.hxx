#pragma once

#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualMultOperator.hxx"

namespace Legolas{
  
  class SparseMultOperatorIgnoreEmptyLines : public SparseVirtualMultOperator{
  public:

    inline std::string name( void ) const { return "SparseMultOperatorIgnoreEmptyLines" ;}
    
    void addMult(const SparseVirtualBlockMatrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y);
    void mult(const SparseVirtualBlockMatrix & A,const VirtualVector & X, VirtualVector & Y);
    
    SparseMultOperatorIgnoreEmptyLines( void );
    ~SparseMultOperatorIgnoreEmptyLines( void );
    SparseMultOperatorIgnoreEmptyLines(const SparseMultOperatorIgnoreEmptyLines & source) ; 

    virtual VirtualMultOperator * clone( void ) const {
      return new SparseMultOperatorIgnoreEmptyLines(*this);
    }
    
  };
}

    
