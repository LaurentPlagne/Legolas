#pragma once

#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualSolver.hxx"

namespace Legolas{
  
  class InPlaceSparseGaussSeidelSolver : public SparseVirtualSolver{
  public:
    
    inline std::string name( void ) const { return "InPlaceSparseGaussSeidelSolver" ;}

    void solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    void transposeSolve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    //    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    InPlaceSparseGaussSeidelSolver( void );
    ~InPlaceSparseGaussSeidelSolver( void );
    InPlaceSparseGaussSeidelSolver(const InPlaceSparseGaussSeidelSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new InPlaceSparseGaussSeidelSolver(*this);
    }
    
  };
}


    
