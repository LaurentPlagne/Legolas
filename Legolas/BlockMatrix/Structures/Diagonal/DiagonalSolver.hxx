#ifndef __LEGOLAS_DIAGONALGAUSSSEIDELSOLVER_HXX__
#define __LEGOLAS_DIAGONALGAUSSSEIDELSOLVER_HXX__

#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualSolver.hxx"

namespace Legolas{
  
  class DiagonalSolver : public DiagonalVirtualSolver{
  public:

    std::string name( void ) const { return "DiagonalSolver";}
    
    void solve(const DiagonalVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    
    DiagonalSolver( void );
    ~DiagonalSolver( void );
    DiagonalSolver(const DiagonalSolver & source) ; 
    
    virtual VirtualSolver * clone( void ) const {
      return new DiagonalSolver(*this);
    }
    
  };
}

#endif
    
