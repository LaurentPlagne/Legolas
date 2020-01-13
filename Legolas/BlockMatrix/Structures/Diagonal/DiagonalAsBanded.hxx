#ifndef __LEGOLAS_DIAGONALASBANDED_HXX__
#define __LEGOLAS_DIAGONALASBANDED_HXX__

namespace Legolas{

  template <class DIAGONAL_MATRIX_INTERFACE>
  struct DiagonalAsBanded : public DIAGONAL_MATRIX_INTERFACE{
    
    typedef typename DIAGONAL_MATRIX_INTERFACE::RealType RealType;
    typedef typename DIAGONAL_MATRIX_INTERFACE::Container Container;
    
    
    DiagonalAsBanded():DIAGONAL_MATRIX_INTERFACE(){}
    DiagonalAsBanded(const VirtualMatrixShape & virtualMatrixShape):DIAGONAL_MATRIX_INTERFACE(virtualMatrixShape){}
    DiagonalAsBanded(const Container & container):DIAGONAL_MATRIX_INTERFACE(container){}

    inline int linf( void ) const { return 0;}
    inline int lsup( void ) const { return 0;}
    inline RealType bandedGetElement(int i, int j) const{ return this->diagonalGetElement(i);}
    inline RealType & bandedGetElement(int i, int j){ return this->diagonalGetElement(i);}
    
  };
}

#endif    
