#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class SteepestDescentSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
  public:
    
    inline std::string name( void ) const { return "SteepestDescentSolver" ;}

    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    SteepestDescentSolver( void );
    ~SteepestDescentSolver( void );
    SteepestDescentSolver(const SteepestDescentSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new SteepestDescentSolver(*this);
    }
    
  };
}

    
