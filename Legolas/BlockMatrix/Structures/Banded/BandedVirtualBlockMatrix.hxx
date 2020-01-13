#ifndef __LEGOLAS_BANDEDVIRTUALBLOCKMATRIX_HXX__
#define __LEGOLAS_BANDEDVIRTUALBLOCKMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedVirtualMultOperator.hxx"

namespace Legolas{

  class BandedVirtualBlockMatrix : public SparseVirtualBlockMatrix {
  public:

    virtual inline std::string name( void ) const { return "BandedMatrix" ;}

    BandedVirtualBlockMatrix(const VirtualMatrixShape & virtualMatrixShape);
    virtual ~BandedVirtualBlockMatrix( void );

    //The overloaded methods are hidden by default. One must use the "using" clause
    //to make the Base class method visible 
    using SparseVirtualBlockMatrix::setSolverPtr;

    virtual void setSolverPtr(BandedVirtualSolver * bandedVirtualSolverPtr);

    using SparseVirtualBlockMatrix::setMultOperatorPtr;
    virtual void setMultOperatorPtr(BandedVirtualMultOperator * bandedVirtualMultOperatorPtr);

    virtual const Matrix & sparseGetElement(int i, int j) const ;
    virtual Matrix & sparseGetElement(int i, int j) ;

    virtual int beginRow( void ) const ;
    virtual int endRow( void ) const ;
    
    virtual int beginColInRow( int i ) const ;
    virtual int endColInRow( int i ) const ;
    virtual int nextColInRow(int i, int j) const ;
    
    virtual const Matrix & bandedGetElement(int i, int j) const =0;
    virtual Matrix & bandedGetElement(int i, int j) =0;
    virtual int linf( void ) const =0;
    virtual int lsup( void ) const =0;
    
  };
  
}

#endif


