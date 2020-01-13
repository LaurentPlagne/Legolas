#ifndef __LEGOLAS_MYSPARSEBLOCKMATRIX_HXX__
#define __LEGOLAS_MYSPARSEBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "MyLightDiagonalMatrix.hxx"

class MySparseBlockMatrix : public Legolas::SparseBlockMatrix {
public:
  MySparseBlockMatrix(const Legolas::MatrixShape<2> ms):Legolas::SparseBlockMatrix(ms){
    //Tridiag Sparsity Pattern
    for (int i=0 ; i<this->nrows() ; i++){
      for (int j=0 ; j<this->ncols() ; j++){
	if (i==j) this->setSparseElementPtr(i,j,new MyLightDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),4.0));
	if (i==(j-1)) this->setSparseElementPtr(i,j,new MyLightDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),1.0));
	if (i==(j+1)) this->setSparseElementPtr(i,j,new MyLightDiagonalMatrix<double>(ms.getSubMatrixShape(i,j),-1.0));
      }
    }
  }
    

  std::string name( void ) const { return "MySparseBlockMatrix" ; }
  
};


#endif
    
    
