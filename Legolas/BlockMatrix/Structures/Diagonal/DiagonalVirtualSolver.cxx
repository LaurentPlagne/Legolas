#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualSolver.hxx"

namespace Legolas{
  void DiagonalVirtualSolver::solve(const Matrix & Av, const VirtualVector & B, VirtualVector & X){
    const DiagonalVirtualBlockMatrix & A=static_cast<const DiagonalVirtualBlockMatrix &>(Av);
    this->solve(A,B,X);
  }
}


