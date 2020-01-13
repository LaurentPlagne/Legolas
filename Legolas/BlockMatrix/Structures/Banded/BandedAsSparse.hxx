#ifndef __LEGOLAS_BANDEDASSPARSE_HXX__
#define __LEGOLAS_BANDEDASSPARSE_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrixInterface.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMatrixOptions.hxx"

namespace Legolas{

  template <class BANDED_MATRIX_INTERFACE>
  struct BandedAsSparse : public BANDED_MATRIX_INTERFACE{
    
    typedef typename BANDED_MATRIX_INTERFACE::RealType RealType;
    typedef typename BANDED_MATRIX_INTERFACE::Container Container;    

    BandedAsSparse():BANDED_MATRIX_INTERFACE(){}
    BandedAsSparse(const VirtualMatrixShape & virtualMatrixShape):BANDED_MATRIX_INTERFACE(virtualMatrixShape){}
    BandedAsSparse(const Container & container):BANDED_MATRIX_INTERFACE(container){}

    inline RealType sparseGetElement(int i, int j) const{ return this->bandedGetElement(i,j);}
    inline RealType & sparseGetElement(int i, int j){ return this->bandedGetElement(i,j);}
    inline int beginRow( void ) const {return 0;}
    inline int endRow( void ) const {return this->nrows();}
    
    inline int beginColInRow( int i ) const { return std::max(i-this->linf(),0);}
    inline int endColInRow( int i ) const { return std::min(i+this->lsup()+1,this->ncols());}
    inline int nextColInRow(int i, int j) const { return j+1 ;}
    
  };
}

#endif    
