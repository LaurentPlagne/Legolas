#ifndef __LEGOLAS_SPARSEJACOBISOLVER_HXX__
#define __LEGOLAS_SPARSEJACOBISOLVER_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualSolver.hxx"

namespace Legolas{
  
  class SparseJacobiSolver : public SparseVirtualSolver{
    VirtualVector * accumulatorPtr_;
    VirtualVector * xnm1Ptr_;
  public:
    
    inline std::string name( void ) const { return "SparseJacobiSolver" ;}

    void solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    //    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    SparseJacobiSolver( void );
    ~SparseJacobiSolver( void );
    SparseJacobiSolver(const SparseJacobiSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new SparseJacobiSolver(*this);
    }
    
  };
}

#endif
    
