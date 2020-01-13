#ifndef __LEGOLAS_MYSPARSEBLOCKMATRIX_HXX__
#define __LEGOLAS_MYSPARSEBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"

class MySparseBlockMatrix : public Legolas::SparseBlockMatrix {
public:
  MySparseBlockMatrix(const Legolas::MatrixShape<2> ms) ;

  std::string name( void ) const { return "MySparseBlockMatrix" ; }
  
};


#endif
    
    
