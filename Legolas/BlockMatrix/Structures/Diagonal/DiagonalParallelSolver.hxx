#ifndef __LEGOLAS_DIAGONALPARALLELSOLVER_HXX__
#define __LEGOLAS_DIAGONALPARALLELSOLVER_HXX__

#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualSolver.hxx"

namespace Legolas{
  
  class DiagonalParallelSolver : public DiagonalVirtualSolver{
  public:

    std::string name( void ) const { return "DiagonalParallelSolver";}
    
    void solve(const DiagonalVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    
    DiagonalParallelSolver( void );
    ~DiagonalParallelSolver( void );
    DiagonalParallelSolver(const DiagonalParallelSolver & source) ; 
    
    virtual VirtualSolver * clone( void ) const {
      return new DiagonalParallelSolver(*this);
    }
    
  };
}

#endif
    
