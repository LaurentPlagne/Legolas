#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class SteepestDescentMinResSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
  public:
    
    inline std::string name( void ) const { return "SteepestDescentMinResSolver" ;}

    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    SteepestDescentMinResSolver( void );
    ~SteepestDescentMinResSolver( void );
    SteepestDescentMinResSolver(const SteepestDescentMinResSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new SteepestDescentMinResSolver(*this);
    }
    
  };
}

    
