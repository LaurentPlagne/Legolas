#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualSolver.hxx"

namespace Legolas{
  void BandedVirtualSolver::solve(const Matrix & Av, const VirtualVector & B, VirtualVector & X){
    const BandedVirtualBlockMatrix & A=static_cast<const BandedVirtualBlockMatrix &>(Av);
    this->solve(A,B,X);
  }
}


