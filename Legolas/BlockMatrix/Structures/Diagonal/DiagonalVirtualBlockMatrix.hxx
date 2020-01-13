#ifndef __LEGOLAS_DIAGONALVIRTUALBLOCKMATRIX_HXX__
#define __LEGOLAS_DIAGONALVIRTUALBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualMultOperator.hxx"

namespace Legolas{
  
  class DiagonalVirtualBlockMatrix : public BandedVirtualBlockMatrix {
  public:
    DiagonalVirtualBlockMatrix(const VirtualMatrixShape & virtualMatrixShape);
    virtual ~DiagonalVirtualBlockMatrix( void );
    
    //The overloaded methods are hidden by default. One must use the "using" clause
    //to make the Bade class method visible 
    //    using SparseVirtualBlockMatrix::setSolverPtr;

    using BandedVirtualBlockMatrix::setSolverPtr;
    virtual void setSolverPtr(DiagonalVirtualSolver * diagonalVirtualSolverPtr);

    using BandedVirtualBlockMatrix::setMultOperatorPtr;
    virtual void setMultOperatorPtr(DiagonalVirtualMultOperator * diagonalVirtualMultOperatorPtr);

    virtual const Matrix & bandedGetElement(int i, int j) const ;
    virtual Matrix & bandedGetElement(int i, int j) ;

    inline virtual int linf( void ) const {return 0;}
    inline virtual int lsup( void ) const {return 0;}
    
    virtual const Matrix & diagonalGetElement(int i) const =0;
    virtual Matrix & diagonalGetElement(int i) =0;
    
  };
  
}

#endif


