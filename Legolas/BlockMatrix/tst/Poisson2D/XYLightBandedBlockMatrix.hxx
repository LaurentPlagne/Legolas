#ifndef __LEGOLAS_XYLIGHTBANDEDBLOCKMATRIX_HXX__
#define __LEGOLAS_XYLIGHTBANDEDBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"
#include "YLightBandedMatrix.hxx"

class XYLightBandedBlockMatrix : public Legolas::BandedBlockMatrix {
public:

  std::string name( void ) const { return "XYLightBandedBlockMatrix" ;}

  XYLightBandedBlockMatrix(const Legolas::MatrixShape<2> ms):BandedBlockMatrix(ms,1,1){
    for (int i=0 ; i<this->nrows() ; i++){
      for (int j=std::max(i-1,0) ; j<std::min(i+2,this->nrows()) ; j++){
	if (i==j  ) this->setBandedElementPtr(i,j,new YLightBandedMatrix<double>(ms.getSubMatrixShape(i,j),true));
	if (i==(j-1)) this->setBandedElementPtr(i,j,new YLightBandedMatrix<double>(ms.getSubMatrixShape(i,j),false));
	if (i==(j+1)) this->setBandedElementPtr(i,j,new YLightBandedMatrix<double>(ms.getSubMatrixShape(i,j),false));
      }
    }
  }
  
};


#endif
    
    
