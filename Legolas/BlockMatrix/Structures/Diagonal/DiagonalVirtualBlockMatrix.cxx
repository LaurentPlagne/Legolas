#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalMultOperator.hxx"

namespace Legolas{

  void DiagonalVirtualBlockMatrix::setSolverPtr(DiagonalVirtualSolver * diagonalVirtualSolverPtr){
    this->setVirtualSolverPtr(diagonalVirtualSolverPtr);
  }

  void DiagonalVirtualBlockMatrix::setMultOperatorPtr(DiagonalVirtualMultOperator * diagonalVirtualMultOperatorPtr){
    this->setVirtualMultOperatorPtr(diagonalVirtualMultOperatorPtr);
  }
  
  DiagonalVirtualBlockMatrix::DiagonalVirtualBlockMatrix(const VirtualMatrixShape & virtualMatrixShape):BandedVirtualBlockMatrix(virtualMatrixShape){
    this->setSolverPtr( new DiagonalSolver());
    this->setMultOperatorPtr( new DiagonalMultOperator());
  }

  DiagonalVirtualBlockMatrix::~DiagonalVirtualBlockMatrix( void ){}

  const Matrix &  DiagonalVirtualBlockMatrix::bandedGetElement(int i, int j) const {
    assert(i==j);
    return this->diagonalGetElement(i);
  }
  Matrix &  DiagonalVirtualBlockMatrix::bandedGetElement(int i, int j){
    assert(i==j);
    return this->diagonalGetElement(i);
  }
  

}

