#include "MyBandedBlockMatrixLevel3.hxx"
#include "MyBandedBlockMatrix.hxx"
#include "MyDiagonalMatrix.hxx"

MyBandedBlockMatrixLevel3::MyBandedBlockMatrixLevel3(const Legolas::MatrixShape<3> ms):BandedBlockMatrix(ms,2,1){
  for (int i=0 ; i<this->nrows() ; i++){
    for (int j=std::max(i-2,0) ; j<=std::min(i+1,this->nrows()-1) ; j++){
      this->setBandedElementPtr(i,j,new MyBandedBlockMatrix(ms.getSubMatrixShape(i,j)));
    }
  }
}



