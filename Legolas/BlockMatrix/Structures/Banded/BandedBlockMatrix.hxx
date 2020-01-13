#ifndef __LEGOLAS_BANDEDBLOCKMATRIX_HXX__
#define __LEGOLAS_BANDEDBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualSolver.hxx"

namespace Legolas{
  
  class BandedBlockMatrix : public BandedVirtualBlockMatrix {

    typedef std::vector< Matrix * >      BandedData;
    BandedData bandedData_;
    int linf_;
    int lsup_;
    int width_;
    
    int getDataIndex(int i, int j) const ;
    
  public:
    
    BandedBlockMatrix(const VirtualMatrixShape & virtualMatrixShape, int linf, int lsup);
    virtual ~BandedBlockMatrix( void ) ;
    
    void setBandedElementPtr(int i, int j, Matrix * matrixPtr);
    
    virtual const Matrix & bandedGetElement(int i, int j) const ;
    virtual Matrix & bandedGetElement(int i, int j) ;
    virtual int linf( void ) const ;
    virtual int lsup( void ) const ;


  };
  
}

#endif


