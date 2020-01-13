#ifndef __LEGOLAS_BANDEDVIRTUALSOLVER_HXX__
#define __LEGOLAS_BANDEDVIRTUALSOLVER_HXX__

#include "Legolas/BlockMatrix/VirtualSolver.hxx"

namespace Legolas{

  class BandedVirtualBlockMatrix;

  class BandedVirtualSolver : public VirtualSolver{
  public:
    virtual void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    virtual void solve(const BandedVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X) = 0;
    
    virtual ~BandedVirtualSolver( void ){};
  };
  
}

#endif

