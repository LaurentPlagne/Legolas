#ifndef __LEGOLAS_SPARSEGAUSSSEIDELSOLVER_HXX__
#define __LEGOLAS_SPARSEGAUSSSEIDELSOLVER_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualSolver.hxx"

namespace Legolas{
  
  class SparseGaussSeidelSolver : public SparseVirtualSolver{
    VirtualVector * accumulatorPtr_;
  public:
    
    inline std::string name( void ) const { return "SparseGaussSeidelSolver" ;}

    void solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    void transposeSolve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    //    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    SparseGaussSeidelSolver( void );
    ~SparseGaussSeidelSolver( void );
    SparseGaussSeidelSolver(const SparseGaussSeidelSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new SparseGaussSeidelSolver(*this);
    }
    
  };
}

#endif
    
