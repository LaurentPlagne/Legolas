#include "MyDiagonalBlockMatrix.hxx"
#include "MyDiagonalMatrix.hxx"
MyDiagonalBlockMatrix::MyDiagonalBlockMatrix(const Legolas::MatrixShape<2> ms):DiagonalBlockMatrix(ms){
  for (int j=0 ; j<this->nrows() ; j++){
    this->setDiagonalElementPtr(j,new MyDiagonalMatrix<double>(ms.getSubMatrixShape(j,j),double(j+1)));
    //    this->diagonalGetElement(j)=new MyDiagonalMatrix<double>(ms.getSubMatrixShape(j,j));
    //this->data()[j]=new MyDiagonalMatrix<double>(ms.getSubMatrixShape(j,j));
  }
}


