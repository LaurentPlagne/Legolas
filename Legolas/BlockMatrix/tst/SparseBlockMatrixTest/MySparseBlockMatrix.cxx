#include "MySparseBlockMatrix.hxx"
#include "MyDiagonalMatrix.hxx"

MySparseBlockMatrix::MySparseBlockMatrix(const Legolas::MatrixShape<2> ms):SparseBlockMatrix(ms){
  this->name()="MySparseBlockMatrix";
  for (int i=0 ; i<this->nrows() ; i++){
    for (int j=0 ; j<this->ncols() ; j++){
      if (i==j) this->setSparseElementPtr(i,j,new MyDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),4.0));
      if (i==(j-1)) this->setSparseElementPtr(i,j,new MyDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),-1.0));
      if (i==(j+1)) this->setSparseElementPtr(i,j,new MyDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),-1.0));
    }
  }
}


