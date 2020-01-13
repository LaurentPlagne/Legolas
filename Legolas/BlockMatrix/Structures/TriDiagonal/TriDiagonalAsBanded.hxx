#ifndef __LEGOLAS_TRIDIAGONALASBANDED_HXX__
#define __LEGOLAS_TRIDIAGONALASBANDED_HXX__

namespace Legolas{

  template <class TRIDIAGONAL_MATRIX_INTERFACE>
  struct TriDiagonalAsBanded : public TRIDIAGONAL_MATRIX_INTERFACE{
    
    typedef typename TRIDIAGONAL_MATRIX_INTERFACE::RealType RealType;
    typedef typename TRIDIAGONAL_MATRIX_INTERFACE::Container Container;
    
    
    TriDiagonalAsBanded():TRIDIAGONAL_MATRIX_INTERFACE(){}
    TriDiagonalAsBanded(const VirtualMatrixShape & virtualMatrixShape):TRIDIAGONAL_MATRIX_INTERFACE(virtualMatrixShape){}
    TriDiagonalAsBanded(const Container & container):TRIDIAGONAL_MATRIX_INTERFACE(container){}

    inline int linf( void ) const { return 1;}
    inline int lsup( void ) const { return 1;}
    
    inline RealType bandedGetElement(int i, int j) const{ 
      if (i==j) return this->diagonalGetElement(i);
      if (j==i+1) return this->upperDiagonalGetElement(i);
      if (j==i-1) return this->lowerDiagonalGetElement(i);
      return 0.0;
    }
    
    inline RealType & bandedGetElement(int i, int j){
      if (i==j){
	return this->diagonalGetElement(i);
      }
      else{
	if (j==i+1){
	    return this->upperDiagonalGetElement(i);
	}
	else {
	  if (j==i-1){
	    return this->lowerDiagonalGetElement(i);
	  }
	  else{
	    INFOS("Invadid TriDiagonalAsBanded indexes");
	    throw std::runtime_error("Invadid TriDiagonalAsBanded indexes");
	    //for compilation warnings
	    return this->diagonalGetElement(i);
	  }
	}
      }
    }
    
  };
}

#endif    
