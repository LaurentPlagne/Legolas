#ifndef __LEGOLAS_BANDEDSCALARMATRIX_HXX__
#define __LEGOLAS_BANDEDSCALARMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrixInterface.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMatrixOptions.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedAsSparse.hxx"

namespace Legolas{

  template<class SCALAR,class BMO>
  struct BandedScalarMatrixTraits{
    typedef typename BMO::BandedContainer::template Engine<SCALAR> T1;
    typedef BandedAsSparse<T1> T2;
    typedef SparseScalarMatrixInterface<T2,BMO> Type;
  };
  
  template <class SCALAR, class BANDED_MATRIX_OPTIONS=BandedScalarMatrixOptions>
  class BandedScalarMatrix : public BandedScalarMatrixTraits<SCALAR,BANDED_MATRIX_OPTIONS>::Type{
  public:

    virtual inline std::string name( void ) const { return "BandedMatrix" ;}

    typedef typename BandedScalarMatrixTraits<SCALAR,BANDED_MATRIX_OPTIONS>::T1 Container;
    typedef typename BandedScalarMatrixTraits<SCALAR,BANDED_MATRIX_OPTIONS>::Type Base;

    BandedScalarMatrix(const BandedScalarMatrix & source):Base(source){};
    BandedScalarMatrix(const VirtualMatrixShape & virtualMatrixShape,int linf, int lsup):Base(virtualMatrixShape){
      this->resize(linf,lsup);
    }
    BandedScalarMatrix(const Container & container):Base(container){}
    
    virtual inline ~BandedScalarMatrix( void ){}

    
  };
}
#endif
