#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundMatrixMultOperator.hxx"
#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundMatrix.hxx"

namespace Legolas{
  void SumCompoundMatrixMultOperator::addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y){
    const SumCompoundMatrix & A=static_cast<const SumCompoundMatrix &>(Av);

    const Matrix & L=A.leftMatrix();
    const Matrix & R=A.rightMatrix();
    
    R.addMult(a,X,Y);
    L.addMult(a,X,Y);

  }
  
  void SumCompoundMatrixMultOperator::mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y){
    const SumCompoundMatrix & A=static_cast<const SumCompoundMatrix &>(Av);

    const Matrix & L=A.leftMatrix();
    const Matrix & R=A.rightMatrix();

    R.mult(X,Y);
    L.addMult(1.0,X,Y);

  }
}


