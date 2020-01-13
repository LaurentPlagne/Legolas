#include "MyBandedBlockMatrix.hxx"
#include "MyDiagonalMatrix.hxx"

MyBandedBlockMatrix::MyBandedBlockMatrix(const Legolas::MatrixShape<2> ms):BandedBlockMatrix(ms,1,1){
  for (int i=0 ; i<this->nrows() ; i++){
    for (int j=std::max(i-1,0) ; j<std::min(i+2,this->nrows()) ; j++){
      if (i==j  ) this->setBandedElementPtr(i,j,new MyDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),4.0));
      if (i==(j-1)) this->setBandedElementPtr(i,j,new MyDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),+1.0));
      if (i==(j+1)) this->setBandedElementPtr(i,j,new MyDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),-1.0));
    }
  }
}



