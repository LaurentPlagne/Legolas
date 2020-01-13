#ifndef __LEGOLAS_SPARSEBLOCKMATRIX_HXX__
#define __LEGOLAS_SPARSEBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualBlockMatrix.hxx"

namespace Legolas{
  
  class SparseBlockMatrix : public SparseVirtualBlockMatrix {

    typedef std::map<int,Matrix*>   Row;
    typedef std::vector< Row >      SparseData;
    
    SparseData sparseData_;
    
  public:
    
    SparseBlockMatrix(const VirtualMatrixShape & virtualMatrixShape);
    virtual ~SparseBlockMatrix( void ) ;
    
    void setSparseElementPtr(int i, int j, Matrix * matrixPtr);
    
    virtual const Matrix & sparseGetElement(int i, int j) const ;
    virtual Matrix & sparseGetElement(int i, int j) ;

    virtual int beginRow( void ) const ;
    virtual int endRow( void ) const ;
    
    virtual int beginColInRow( int i ) const ;
    virtual int endColInRow( int i ) const ;
    
    virtual int nextColInRow(int i, int j) const ;
    
  };
  
}

#endif


