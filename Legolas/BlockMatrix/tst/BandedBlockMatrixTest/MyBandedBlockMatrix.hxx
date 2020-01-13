#ifndef __LEGOLAS_MYBANDEDBLOCKMATRIX_HXX__
#define __LEGOLAS_MYBANDEDBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"

class MyBandedBlockMatrix : public Legolas::BandedBlockMatrix {
public:

  std::string name( void ) const { return "MyBandedBlockMatrix" ;}

  MyBandedBlockMatrix(const Legolas::MatrixShape<2> ms) ;
};


#endif
    
    
