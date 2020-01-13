#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class RestartedPreconditionedConjugateGradientSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
    VirtualVector * PPtr_;
    VirtualVector * ZPtr_;
    Matrix * preconditioner_;
    int counter_;
  public:

    virtual std::string name( void ) const ;
    
    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    RestartedPreconditionedConjugateGradientSolver( void );
    RestartedPreconditionedConjugateGradientSolver(Matrix * RestartedPreconditioner);
    RestartedPreconditionedConjugateGradientSolver(const RestartedPreconditionedConjugateGradientSolver & source) ; 
    ~RestartedPreconditionedConjugateGradientSolver( void );

    virtual VirtualSolver * clone( void ) const {
      return new RestartedPreconditionedConjugateGradientSolver(*this);
    }

    bool restart( void );
    
  };
}

    
