#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalBlockMatrix.hxx"

namespace Legolas{
  
  DiagonalBlockMatrix::DiagonalBlockMatrix(const VirtualMatrixShape & virtualMatrixShape):DiagonalVirtualBlockMatrix(virtualMatrixShape),
											  diagonalData_()
  {
    diagonalData_.resize(this->nrows(),0);
  }

  DiagonalBlockMatrix::~DiagonalBlockMatrix( void ){
    for (int i=0 ; i< this->nrows() ; i++){
      if (diagonalData_[i]){
	delete diagonalData_[i];
	diagonalData_[i]=0;
      }
    }
  }
  

  void DiagonalBlockMatrix::setDiagonalElementPtr(int i, Matrix * matrixPtr){
    diagonalData_[i]=matrixPtr;
  }
  
  const Matrix & DiagonalBlockMatrix::diagonalGetElement( int i ) const {
    return *diagonalData_[i];
  }

  Matrix & DiagonalBlockMatrix::diagonalGetElement( int i ){
    return *diagonalData_[i];
  }

}

