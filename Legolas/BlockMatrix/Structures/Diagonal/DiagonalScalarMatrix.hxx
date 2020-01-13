#ifndef __LEGOLAS_DIAGONALSCALARMATRIX_HXX__
#define __LEGOLAS_DIAGONALSCALARMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrixInterface.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedAsSparse.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrixOptions.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalAsBanded.hxx"


namespace Legolas{

  template<class SCALAR,class BMO>
  struct DiagonalScalarMatrixTraits{
    typedef typename BMO::DiagonalContainer::template Engine<SCALAR> T1;
    typedef DiagonalAsBanded<T1> T2;
    typedef BandedAsSparse<T2> T3;
    typedef SparseScalarMatrixInterface<T3,BMO> Type;
  };
  
  template <class SCALAR, class DIAGONAL_MATRIX_OPTIONS=DiagonalScalarMatrixOptions>
  class DiagonalScalarMatrix : public DiagonalScalarMatrixTraits<SCALAR,DIAGONAL_MATRIX_OPTIONS>::Type{
  public:

    typedef typename DiagonalScalarMatrixTraits<SCALAR,DIAGONAL_MATRIX_OPTIONS>::T1 Container;
    
    virtual inline std::string name( void ) const {return "MyDiagonalMatrix";}
    
    typedef typename DiagonalScalarMatrixTraits<SCALAR,DIAGONAL_MATRIX_OPTIONS>::Type Base;
    DiagonalScalarMatrix():Base(){};
    DiagonalScalarMatrix(const DiagonalScalarMatrix & source):Base(source){};
    DiagonalScalarMatrix(const VirtualMatrixShape & virtualMatrixShape):Base(virtualMatrixShape){}
    DiagonalScalarMatrix(const Container & container):Base(container){}

    virtual inline ~DiagonalScalarMatrix( void ){}
    
  };
  
}
#endif
