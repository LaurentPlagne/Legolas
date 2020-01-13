#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class EnforcedPreconditionedConjugateGradientSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
    VirtualVector * PPtr_;
    VirtualVector * ZPtr_;
    VirtualVector * BPtr_;
    Matrix * preconditioner_;
    int counter_;
  public:

    virtual std::string name( void ) const ;
    
    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    EnforcedPreconditionedConjugateGradientSolver( void );
    EnforcedPreconditionedConjugateGradientSolver(Matrix * EnforcedPreconditioner);
    EnforcedPreconditionedConjugateGradientSolver(const EnforcedPreconditionedConjugateGradientSolver & source) ; 
    ~EnforcedPreconditionedConjugateGradientSolver( void );

    virtual VirtualSolver * clone( void ) const {
      return new EnforcedPreconditionedConjugateGradientSolver(*this);
    }

    bool goon(const VirtualVector & R);
    
  };
}

    
