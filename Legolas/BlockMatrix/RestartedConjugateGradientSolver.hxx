#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class RestartedConjugateGradientSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
    VirtualVector * PPtr_;
    int counter_;
  public:
    
    inline std::string name( void ) const { return "RestartedConjugateGradientSolver" ;}

    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    RestartedConjugateGradientSolver( void );
    ~RestartedConjugateGradientSolver( void );
    RestartedConjugateGradientSolver(const RestartedConjugateGradientSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new RestartedConjugateGradientSolver(*this);
    }

    bool restart( void );
    
  };
}
    
