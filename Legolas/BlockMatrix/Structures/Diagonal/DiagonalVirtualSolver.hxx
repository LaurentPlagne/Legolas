#ifndef __LEGOLAS_DIAGONALVIRTUALSOLVER_HXX__
#define __LEGOLAS_DIAGONALVIRTUALSOLVER_HXX__

#include "Legolas/BlockMatrix/VirtualSolver.hxx"

namespace Legolas{

  class DiagonalVirtualBlockMatrix;

  class DiagonalVirtualSolver : public VirtualSolver{
  public:
    virtual void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    virtual void solve(const DiagonalVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X) = 0;
    
    virtual ~DiagonalVirtualSolver( void ){};
  };
  
}

#endif

