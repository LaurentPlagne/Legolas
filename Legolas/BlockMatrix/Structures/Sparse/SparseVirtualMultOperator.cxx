#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualMultOperator.hxx"

namespace Legolas{
  void SparseVirtualMultOperator:: addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y){
    const SparseVirtualBlockMatrix & A=static_cast<const SparseVirtualBlockMatrix &>(Av);
    this->addMult(A,a,X,Y);
  }

  void SparseVirtualMultOperator:: mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y){
    const SparseVirtualBlockMatrix & A=static_cast<const SparseVirtualBlockMatrix &>(Av);
    this->mult(A,X,Y);
  }

  void SparseVirtualMultOperator:: transposeAddMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y){
    const SparseVirtualBlockMatrix & A=static_cast<const SparseVirtualBlockMatrix &>(Av);
    this->transposeAddMult(A,a,X,Y);
  }

  void SparseVirtualMultOperator:: transposeMult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y){
    const SparseVirtualBlockMatrix & A=static_cast<const SparseVirtualBlockMatrix &>(Av);
    this->transposeMult(A,X,Y);
  }
}


