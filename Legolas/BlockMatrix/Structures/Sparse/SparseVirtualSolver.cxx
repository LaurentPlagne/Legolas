
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualSolver.hxx"

namespace Legolas{
  void SparseVirtualSolver::solve(const Matrix & Av, const VirtualVector & B, VirtualVector & X){
    const SparseVirtualBlockMatrix & A=static_cast<const SparseVirtualBlockMatrix &>(Av);
    this->solve(A,B,X);
  }

  void SparseVirtualSolver::transposeSolve(const Matrix & Av, const VirtualVector & B, VirtualVector & X){
    const SparseVirtualBlockMatrix & A=static_cast<const SparseVirtualBlockMatrix &>(Av);
    this->transposeSolve(A,B,X);
  }
}


