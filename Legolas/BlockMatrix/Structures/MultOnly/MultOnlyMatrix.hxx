#pragma once

#include "Legolas/BlockMatrix/Structures/MultOnly/MultOnlyMatrixInterface.hxx"

namespace Legolas{
  
  template <class SCALAR, int LEVEL>
  class MultOnlyMatrix : public MultOnlyMatrixInterface<SCALAR,LEVEL>{
  public:
    
    typedef MultOnlyMatrixInterface<SCALAR,LEVEL> Base;
    MultOnlyMatrix(const MultOnlyMatrix & source):Base(source){};
    MultOnlyMatrix(const VirtualMatrixShape & virtualMatrixShape):Base(virtualMatrixShape){
    }
    
    virtual inline ~MultOnlyMatrix( void ){}
    
    
  };
  
}

    
    
    
    
