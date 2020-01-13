#ifndef __LEGOLAS_SUMCOMPOUNDMATRIXMULTOPERATOR_HXX__
#define __LEGOLAS_SUMCOMPOUNDMATRIXMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"

namespace Legolas{
  
  class SumCompoundMatrixMultOperator : public VirtualMultOperator{
  public:
    
    void addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y);
    void mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y);
    
    virtual ~SumCompoundMatrixMultOperator( void ){};

    virtual VirtualMultOperator * clone( void ) const {
      return new SumCompoundMatrixMultOperator(*this);
    }
  
    inline std::string name( void ) const { return "SumCompoundMultOperator" ;}
  
  };
  
}

#endif
