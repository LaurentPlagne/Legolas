#ifndef __LEGOLAS_TRIDIAGONALSCALARMATRIX_HXX__
#define __LEGOLAS_TRIDIAGONALSCALARMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrixInterface.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedAsSparse.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarMatrixOptions.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalAsBanded.hxx"


namespace Legolas{

  template<class SCALAR,class BMO>
  struct TriDiagonalScalarMatrixTraits{
    typedef typename BMO::TriDiagonalContainer::template Engine<SCALAR> T1;
    typedef TriDiagonalAsBanded<T1> T2;
    typedef BandedAsSparse<T2> T3;
    typedef SparseScalarMatrixInterface<T3,BMO> Type;
  };
  
  template <class SCALAR, class TRIDIAGONAL_MATRIX_OPTIONS=TriDiagonalScalarMatrixOptions>
  class TriDiagonalScalarMatrix : public TriDiagonalScalarMatrixTraits<SCALAR,TRIDIAGONAL_MATRIX_OPTIONS>::Type{
  public:

    typedef typename TriDiagonalScalarMatrixTraits<SCALAR,TRIDIAGONAL_MATRIX_OPTIONS>::T1 Container;
    
    virtual inline std::string name( void ) const {return "MyTriDiagonalMatrix";}
    
    typedef typename TriDiagonalScalarMatrixTraits<SCALAR,TRIDIAGONAL_MATRIX_OPTIONS>::Type Base;
    TriDiagonalScalarMatrix():Base(){};
    TriDiagonalScalarMatrix(const TriDiagonalScalarMatrix & source):Base(source){};
    TriDiagonalScalarMatrix(const VirtualMatrixShape & virtualMatrixShape):Base(virtualMatrixShape){}
    TriDiagonalScalarMatrix(const Container & container):Base(container){}

    virtual inline ~TriDiagonalScalarMatrix( void ){}

    
  };
  
}
#endif
