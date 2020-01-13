#ifndef __LEGOLAS_CONJUGATEGRADIENTSOLVER_HXX__
#define __LEGOLAS_CONJUGATEGRADIENTSOLVER_HXX__

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class ConjugateGradientSolver : public MatrixSolver{
    VirtualVector * RPtr_;
    VirtualVector * DPtr_;
    VirtualVector * PPtr_;
  public:
    
    inline std::string name( void ) const { return "ConjugateGradientSolver" ;}

    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    ConjugateGradientSolver( void );
    ~ConjugateGradientSolver( void );
    ConjugateGradientSolver(const ConjugateGradientSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new ConjugateGradientSolver(*this);
    }
    
  };
}

#endif
    
