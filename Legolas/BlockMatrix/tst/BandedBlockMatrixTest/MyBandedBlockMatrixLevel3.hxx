#ifndef __LEGOLAS_MYBANDEDBLOCKMATRIXLEVEL3_HXX__
#define __LEGOLAS_MYBANDEDBLOCKMATRIXLEVEL3_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"

class MyBandedBlockMatrixLevel3 : public Legolas::BandedBlockMatrix {
public:

  std::string name( void ) const { return "MyBandedBlockMatrixLevel3" ;}

  MyBandedBlockMatrixLevel3(const Legolas::MatrixShape<3> ms) ;
};


#endif
    
    
