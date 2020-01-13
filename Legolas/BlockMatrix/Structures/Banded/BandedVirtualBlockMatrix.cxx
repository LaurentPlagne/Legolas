#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedMultOperator.hxx"

namespace Legolas{

  void BandedVirtualBlockMatrix::setSolverPtr(BandedVirtualSolver * bandedVirtualSolverPtr){
    this->setVirtualSolverPtr(bandedVirtualSolverPtr);
  }

  void BandedVirtualBlockMatrix::setMultOperatorPtr(BandedVirtualMultOperator * bandedVirtualMultOperatorPtr){
    this->setVirtualMultOperatorPtr(bandedVirtualMultOperatorPtr);
  }

  BandedVirtualBlockMatrix::BandedVirtualBlockMatrix(const VirtualMatrixShape & virtualMatrixShape):SparseVirtualBlockMatrix(virtualMatrixShape){
    this->setSolverPtr( new BandedGaussSeidelSolver());
    this->setMultOperatorPtr( new BandedMultOperator());
  }

  BandedVirtualBlockMatrix::~BandedVirtualBlockMatrix( void ){}

  const Matrix &  BandedVirtualBlockMatrix::sparseGetElement(int i, int j) const {
    return this->bandedGetElement(i,j);
  }
  Matrix &  BandedVirtualBlockMatrix::sparseGetElement(int i, int j){
    return this->bandedGetElement(i,j);
  }
  
  int BandedVirtualBlockMatrix::beginRow( void ) const { return 0;}
  int BandedVirtualBlockMatrix::endRow( void ) const { return this->nrows();}
  int BandedVirtualBlockMatrix::beginColInRow( int i ) const { return std::max(i-this->linf(),0);}
  int BandedVirtualBlockMatrix::endColInRow( int i ) const { return std::min(i+this->lsup()+1,this->nrows());}
  int BandedVirtualBlockMatrix::nextColInRow(int i, int j) const { return j+1;}
  
}
