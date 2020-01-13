#ifndef __LEGOLAS_XYBANDEDPOLYBLOCKMATRIX_HXX__
#define __LEGOLAS_XYBANDEDPOLYBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"
#include "YLightDiagonalMatrix.hxx"
#include "YLightTriDiagonalMatrix.hxx"

class XYBandedPolyBlockMatrix : public Legolas::BandedBlockMatrix {
public:

  std::string name( void ) const { return "XYBandedPolyBlockMatrix" ;}

  XYBandedPolyBlockMatrix(const Legolas::MatrixShape<2> ms):BandedBlockMatrix(ms,1,1){
    for (int i=0 ; i<this->nrows() ; i++){
      for (int j=std::max(i-1,0) ; j<std::min(i+2,this->nrows()) ; j++){
	if (i==j  ) this->setBandedElementPtr(i,j,new YLightTriDiagonalMatrix<double>(ms.getSubMatrixShape(i,j)));
	if (i==(j-1)) this->setBandedElementPtr(i,j,new YLightDiagonalMatrix<double>(ms.getSubMatrixShape(i,j)));
	if (i==(j+1)) this->setBandedElementPtr(i,j,new YLightDiagonalMatrix<double>(ms.getSubMatrixShape(i,j)));
      }
    }
  }
  
};


#endif
    
    
