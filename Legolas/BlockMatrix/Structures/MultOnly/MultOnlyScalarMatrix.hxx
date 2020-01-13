#ifndef __LEGOLAS_MULTONLYSCALARMATRIX_HXX__
#define __LEGOLAS_MULTONLYSCALARMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/MultOnly/MultOnlyScalarMatrixInterface.hxx"

namespace Legolas{
  
  template <class SCALAR>
  class MultOnlyScalarMatrix : public MultOnlyScalarMatrixInterface<SCALAR> {
  public:
    
    typedef MultOnlyScalarMatrixInterface<SCALAR> Base;
    MultOnlyScalarMatrix(const MultOnlyScalarMatrix & source):Base(source){};
    MultOnlyScalarMatrix(const VirtualMatrixShape & virtualMatrixShape):Base(virtualMatrixShape){
    }
    
    virtual inline ~MultOnlyScalarMatrix( void ){}
    
    
  };
  
}

#endif
    
    
    
    
