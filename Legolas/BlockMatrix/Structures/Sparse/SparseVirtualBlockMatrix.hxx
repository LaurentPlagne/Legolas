#ifndef __LEGOLAS_SPARSEVIRTUALBLOCKMATRIX_HXX__
#define __LEGOLAS_SPARSEVIRTUALBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualMultOperator.hxx"

namespace Legolas{
  
  class SparseVirtualBlockMatrix : public Matrix {
  public:
    
    SparseVirtualBlockMatrix(const VirtualMatrixShape & virtualMatrixShape) ; 
    virtual ~SparseVirtualBlockMatrix( void ) ;
 
    using Matrix::setSolverPtr ;

    virtual void setSolverPtr(SparseVirtualSolver * sparseVirtualSolverPtr) ;
    virtual void setMultOperatorPtr(SparseVirtualMultOperator * sparseVirtualMultOperatorPtr) ;

    virtual void iterateOverElements(Legolas::MatrixStream & matrixStream) const ;

    virtual void collectStatistics(Profile & profile) const;

    virtual int getPrecisionInBytePerRealElement( void ) const ;

    virtual std::string name( void ) const ;
    
    virtual const Matrix & sparseGetElement(int i, int j) const =0;
    virtual Matrix & sparseGetElement(int i, int j) =0;
    
    virtual int beginRow( void ) const =0;
    virtual int endRow( void ) const =0;
    
    virtual int beginColInRow( int i ) const =0;
    virtual int endColInRow( int i ) const =0;
    
    virtual int nextColInRow(int i, int j) const =0;

  };
  
}

#endif


