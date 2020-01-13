#ifndef __LEGOLAS_BANDEDGAUSSSEIDELSOLVER_HXX__
#define __LEGOLAS_BANDEDGAUSSSEIDELSOLVER_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualSolver.hxx"

namespace Legolas{
  
  class BandedGaussSeidelSolver : public BandedVirtualSolver{
    VirtualVector * accumulatorPtr_;
  public:

    std::string name( void ) const { return "BandedGaussSeidelSolver";}

    void solve(const BandedVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);

    BandedGaussSeidelSolver( void );
    ~BandedGaussSeidelSolver( void );
    BandedGaussSeidelSolver(const BandedGaussSeidelSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new BandedGaussSeidelSolver(*this);
    }
    
  };
}

#endif
    
