#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualMultOperator.hxx"

namespace Legolas{
  void BandedVirtualMultOperator:: addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y){
    const BandedVirtualBlockMatrix & A=static_cast<const BandedVirtualBlockMatrix &>(Av);
    this->addMult(A,a,X,Y);
  }
  void BandedVirtualMultOperator:: mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y){
    const BandedVirtualBlockMatrix & A=static_cast<const BandedVirtualBlockMatrix &>(Av);
    this->mult(A,X,Y);
  }
}
