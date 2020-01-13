#include "MyDiagonalBlockMatrix.hxx"
#include "MyTriDiagonalMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarDirectSolver.hxx"
#include "RealType.hxx"


MyDiagonalBlockMatrix::MyDiagonalBlockMatrix(const Legolas::MatrixShape<2> ms):DiagonalBlockMatrix(ms){
  for (int j=0 ; j<this->nrows() ; j++){
    MyTriDiagonalMatrix<RealType> * tdPtr=new MyTriDiagonalMatrix<RealType>(ms.getSubMatrixShape(j,j),j);
    typedef Legolas::TriDiagonalScalarDirectSolver::Engine<MyTriDiagonalMatrix<RealType> > TDSDS;
    tdPtr->setSolverPtr( new TDSDS() );						
    this->setDiagonalElementPtr(j,tdPtr);
  }
}


