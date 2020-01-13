#ifndef __LEGOLAS_MATRIXSOLVER_HXX__
#define __LEGOLAS_MATRIXSOLVER_HXX__

#include "UTILITES.hxx"
#include "Legolas/BlockMatrix/VirtualSolver.hxx"

namespace Legolas{
  
  class Matrix;
  
  class MatrixSolver: public VirtualSolver{
  public:
    MatrixSolver( void ){};
    virtual inline ~MatrixSolver( void ){};
    virtual VirtualSolver * clone( void ) const = 0 ;
    virtual void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X) = 0 ;
    virtual void transposeSolve(const Matrix & A, const VirtualVector & B, VirtualVector & X){
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }
  };
  
}

#endif
    
