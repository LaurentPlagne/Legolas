#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class AdaptivePreconditionedConjugateGradientSolver : public MatrixSolver{
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
    
    AdaptivePreconditionedConjugateGradientSolver( void );
    AdaptivePreconditionedConjugateGradientSolver(Matrix * AdaptivePreconditioner);
    AdaptivePreconditionedConjugateGradientSolver(const AdaptivePreconditionedConjugateGradientSolver & source) ; 
    ~AdaptivePreconditionedConjugateGradientSolver( void );

    virtual VirtualSolver * clone( void ) const {
      return new AdaptivePreconditionedConjugateGradientSolver(*this);
    }

    bool restart(const VirtualVector & B,const VirtualVector & R);
    
  };
}

    
