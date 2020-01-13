#ifndef __LEGOLAS_DIAGONALBLOCKMATRIX_HXX__
#define __LEGOLAS_DIAGONALBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualSolver.hxx"

namespace Legolas{
  
  class DiagonalBlockMatrix : public DiagonalVirtualBlockMatrix {

    typedef std::vector< Matrix * >      DiagonalData;
    DiagonalData diagonalData_;
    
  public:

    virtual inline std::string name( void ) const { return "DiagonalBlockMatrix";}
    
    DiagonalBlockMatrix(const VirtualMatrixShape & virtualMatrixShape);
    virtual ~DiagonalBlockMatrix( void ) ;
    
    void setDiagonalElementPtr(int i, Matrix * matrixPtr);
    
    virtual const Matrix & diagonalGetElement(int i) const ;
    virtual Matrix & diagonalGetElement(int i) ;

  };
  
}

#endif


