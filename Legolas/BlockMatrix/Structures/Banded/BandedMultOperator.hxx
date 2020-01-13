#ifndef __LEGOLAS_BANDEDMULTOPERATOR_HXX__
#define __LEGOLAS_BANDEDMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualMultOperator.hxx"

namespace Legolas{
  
  class BandedMultOperator : public BandedVirtualMultOperator{
  public:
    
    virtual inline std::string name( void ) const { return "BandedMultOperator";}

    void addMult(const BandedVirtualBlockMatrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y);
    void mult(const BandedVirtualBlockMatrix & A ,const VirtualVector & X, VirtualVector & Y);
    
    BandedMultOperator( void );
    ~BandedMultOperator( void );
    BandedMultOperator(const BandedMultOperator & source) ; 

    virtual VirtualMultOperator * clone( void ) const {
      return new BandedMultOperator(*this);
    }
    
  };
}

#endif
    
