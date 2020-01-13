#ifndef __LEGOLAS_SPARSEVIRTUALSOLVER_HXX__
#define __LEGOLAS_SPARSEVIRTUALSOLVER_HXX__

#include "Legolas/BlockMatrix/VirtualSolver.hxx"

namespace Legolas{

  class SparseVirtualBlockMatrix;
  
  class SparseVirtualSolver : public VirtualSolver{
  public:

    virtual void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    virtual void solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X) = 0;

    virtual void transposeSolve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    virtual void transposeSolve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){
      assert(1==0);
      INFOS("Not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }

    virtual ~SparseVirtualSolver( void ){};
  };
  
}

#endif

