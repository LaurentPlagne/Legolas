#pragma once

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class RobustPreconditionedConjugateGradientSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
    VirtualVector * PPtr_;
    VirtualVector * ZPtr_;
    VirtualVector * BPtr_;
    Matrix * preconditioner_;
    int counter_;
    double rmax_;
  public:

    virtual std::string name( void ) const ;
    
    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    RobustPreconditionedConjugateGradientSolver( void );
    RobustPreconditionedConjugateGradientSolver(Matrix * RobustPreconditioner);
    RobustPreconditionedConjugateGradientSolver(const RobustPreconditionedConjugateGradientSolver & source) ; 
    ~RobustPreconditionedConjugateGradientSolver( void );

    virtual VirtualSolver * clone( void ) const {
      return new RobustPreconditionedConjugateGradientSolver(*this);
    }

    bool restart(const VirtualVector & B,const VirtualVector & R);
    
  };
}

    
