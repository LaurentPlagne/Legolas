#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class BiCGStabSolver : public MatrixSolver{
    VirtualVector * RkPtr_;
    VirtualVector * RtkPtr_;

    VirtualVector * PkPtr_;
    VirtualVector * PtkPtr_;

    VirtualVector * QkPtr_;
    VirtualVector * QtkPtr_;

  public:
    
    inline std::string name( void ) const { return "BiCGStabSolver" ;}

    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    BiCGStabSolver( void );
    ~BiCGStabSolver( void );
    BiCGStabSolver(const BiCGStabSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new BiCGStabSolver(*this);
    }
    
  };
}

    
