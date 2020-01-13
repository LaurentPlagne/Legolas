#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualMultOperator.hxx"

namespace Legolas{
  void DiagonalVirtualMultOperator:: addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y){
    const DiagonalVirtualBlockMatrix & A=static_cast<const DiagonalVirtualBlockMatrix &>(Av);
    this->addMult(A,a,X,Y);
  }
  void DiagonalVirtualMultOperator:: mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y){
    const DiagonalVirtualBlockMatrix & A=static_cast<const DiagonalVirtualBlockMatrix &>(Av);
    this->mult(A,X,Y);
  }
}
