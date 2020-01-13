#ifndef __LEGOLAS_DIAGONALVIRTUALMULTOPERATOR_HXX__
#define __LEGOLAS_DIAGONALVIRTUALMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"

namespace Legolas{

  class DiagonalVirtualBlockMatrix;
  
  class DiagonalVirtualMultOperator : public VirtualMultOperator{
  public:

    virtual void addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y);
    virtual void addMult(const DiagonalVirtualBlockMatrix & A, const double & a, const VirtualVector & X, VirtualVector & Y) = 0;

    virtual void mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y);
    virtual void mult(const DiagonalVirtualBlockMatrix & A, const VirtualVector & X, VirtualVector & Y) = 0;

    virtual ~DiagonalVirtualMultOperator( void ){};
  };
  
}

#endif
