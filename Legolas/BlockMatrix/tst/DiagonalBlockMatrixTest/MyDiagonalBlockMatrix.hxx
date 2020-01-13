#ifndef __LEGOLAS_MYDIAGONALBLOCKMATRIX_HXX__
#define __LEGOLAS_MYDIAGONALBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalBlockMatrix.hxx"
#include "MyDiagonalMatrix.hxx"

class MyDiagonalBlockMatrix : public Legolas::DiagonalBlockMatrix {
public:

  MyDiagonalBlockMatrix(const Legolas::MatrixShape<2> ms) ;
};


#endif
    
    
