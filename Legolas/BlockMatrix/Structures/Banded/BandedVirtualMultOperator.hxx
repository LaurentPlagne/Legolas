#ifndef __LEGOLAS_BANDEDVIRTUALMULTOPERATOR_HXX__
#define __LEGOLAS_BANDEDVIRTUALMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"

namespace Legolas{

  class BandedVirtualBlockMatrix;
  
  class BandedVirtualMultOperator : public VirtualMultOperator{
  public:

    virtual void addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y);
    //    virtual void mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y);
    virtual void mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y);
    virtual void addMult(const BandedVirtualBlockMatrix & A, const double & a, const VirtualVector & X, VirtualVector & Y) = 0;

    virtual ~BandedVirtualMultOperator( void ){};
  };
  
}

#endif
