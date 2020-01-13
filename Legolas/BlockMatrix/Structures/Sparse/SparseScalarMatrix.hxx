#ifndef __LEGOLAS_SPARSESCALARMATRIX_HXX__
#define __LEGOLAS_SPARSESCALARMATRIX_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrixInterface.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrixOptions.hxx"

namespace Legolas{

  template <class SCALAR,class SMO>
  struct SparseScalarMatrixTraits{
    typedef typename SMO::SparseContainer::template Engine<SCALAR> InnerType;
    typedef SparseScalarMatrixInterface<InnerType,SMO> Type;
  };
  
  template <class SCALAR, class SPARSE_MATRIX_OPTIONS=SparseScalarMatrixOptions>
  class SparseScalarMatrix : public SparseScalarMatrixTraits<SCALAR,SPARSE_MATRIX_OPTIONS>::Type{
  public:

    typedef typename SparseScalarMatrixTraits<SCALAR,SPARSE_MATRIX_OPTIONS>::Type Base;
    SparseScalarMatrix(const SparseScalarMatrix & source):Base(source){};
    SparseScalarMatrix(const VirtualMatrixShape & virtualMatrixShape):Base(virtualMatrixShape){
    }

    virtual inline ~SparseScalarMatrix( void ){}
    
    
  };
  
}

#endif
    
    
    
    
