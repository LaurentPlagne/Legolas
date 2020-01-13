#ifndef __LEGOLAS_XYDIAGONALBLOCKMATRIX_HXX__
#define __LEGOLAS_XYDIAGONALBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalBlockMatrix.hxx"
#include "YLightTriDiagonalMatrix.hxx"
//#include "YLightDiagonal4Matrix.hxx"

class XYDiagonalBlockMatrix : public Legolas::DiagonalBlockMatrix {
public:

  std::string name( void ) const { return "XYDiagonalBlockMatrix" ;}

  XYDiagonalBlockMatrix(const Legolas::MatrixShape<2> ms):Legolas::DiagonalBlockMatrix(ms){
    for (int i=0 ; i<this->nrows() ; i++){
      //      this->setDiagonalElementPtr(i,new YLightDiagonal4Matrix<double>(ms.getSubMatrixShape(i,i)));
      this->setDiagonalElementPtr(i,new YLightTriDiagonalMatrix<double>(ms.getSubMatrixShape(i,i)));
    }
  }
  
};


#endif
    
    
