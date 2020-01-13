#ifndef __LEGOLAS_SUMCOMPOUNDSOLVER_HXX__
#define __LEGOLAS_SUMCOMPOUNDSOLVER_HXX__

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class SumCompoundSolver : public MatrixSolver{
    VirtualVector * SPtr_;
  public:
    
    inline std::string name( void ) const { return "SumCompoundSolver" ;}

    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    
    SumCompoundSolver( void );
    ~SumCompoundSolver( void );
    SumCompoundSolver(const SumCompoundSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new SumCompoundSolver(*this);
    }
    
  };
}

#endif
    
