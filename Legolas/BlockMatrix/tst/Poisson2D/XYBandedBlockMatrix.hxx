#ifndef __LEGOLAS_XYBANDEDBLOCKMATRIX_HXX__
#define __LEGOLAS_XYBANDEDBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"
#include "YBandedMatrix.hxx"

class XYBandedBlockMatrix : public Legolas::BandedBlockMatrix {
public:

  std::string name( void ) const { return "XYBandedBlockMatrix" ;}

  XYBandedBlockMatrix(const Legolas::MatrixShape<2> ms):BandedBlockMatrix(ms,1,1){
    for (int i=0 ; i<this->nrows() ; i++){
      for (int j=std::max(i-1,0) ; j<std::min(i+2,this->nrows()) ; j++){
	if (i==j  ) this->setBandedElementPtr(i,j,new YBandedMatrix<double>(ms.getSubMatrixShape(i,j),true));
	if (i==(j-1)) this->setBandedElementPtr(i,j,new YBandedMatrix<double>(ms.getSubMatrixShape(i,j),false));
	if (i==(j+1)) this->setBandedElementPtr(i,j,new YBandedMatrix<double>(ms.getSubMatrixShape(i,j),false));
      }
    }
  }
  
};


#endif
    
    
