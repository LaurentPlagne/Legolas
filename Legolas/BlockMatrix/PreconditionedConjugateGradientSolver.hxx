#ifndef __LEGOLAS_PRECONDITIONEDCONJUGATEGRADIENTSOLVER_HXX__
#define __LEGOLAS_PRECONDITIONEDCONJUGATEGRADIENTSOLVER_HXX__

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class PreconditionedConjugateGradientSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
    VirtualVector * PPtr_;
    VirtualVector * ZPtr_;
    Matrix * preconditioner_;
  public:

    virtual std::string name( void ) const ;
    
    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    PreconditionedConjugateGradientSolver( void );
    PreconditionedConjugateGradientSolver(Matrix * Preconditioner);
    PreconditionedConjugateGradientSolver(const PreconditionedConjugateGradientSolver & source) ; 
    ~PreconditionedConjugateGradientSolver( void );

    virtual VirtualSolver * clone( void ) const {
      return new PreconditionedConjugateGradientSolver(*this);
    }
    
  };
}

#endif
    
