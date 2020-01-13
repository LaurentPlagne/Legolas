#include "Legolas/BlockMatrix/Structures/ProductCompound/ProductCompoundMatrixMultOperator.hxx"
#include "Legolas/BlockMatrix/Structures/ProductCompound/ProductCompoundMatrix.hxx"

namespace Legolas{
  void ProductCompoundMatrixMultOperator::addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y){
    const ProductCompoundMatrix & A=static_cast<const ProductCompoundMatrix &>(Av);

    const Matrix & L=A.leftMatrix();
    const Matrix & R=A.rightMatrix();
    
    VirtualVector * RXPtr=Y.newVirtualVector(R.getVirtualMatrixShape().getRowShape());
    VirtualVector & RX = *RXPtr;

    R.mult(X,RX);
    L.addMult(a,RX,Y);
    delete RXPtr;
  }
  
  void ProductCompoundMatrixMultOperator::mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y){
    const ProductCompoundMatrix & A=static_cast<const ProductCompoundMatrix &>(Av);

    const Matrix & L=A.leftMatrix();
    const Matrix & R=A.rightMatrix();

    VirtualVector * RXPtr(Y.newVirtualVector(R.getVirtualMatrixShape().getRowShape()));
    VirtualVector & RX = *RXPtr;

    R.mult(X,RX);
    L.mult(RX,Y);
    delete RXPtr;
  }
}


