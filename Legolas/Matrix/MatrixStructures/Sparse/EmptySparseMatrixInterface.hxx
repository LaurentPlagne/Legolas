#ifndef _LEGOLAS__EMPTYSPARSEMATRIXINTERFACE_HXX__
#define _LEGOLAS__EMPTYSPARSEMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/EmptySparseMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"

namespace Legolas{

  template <class ELEMENT_MATRIX_INTERFACE>
  class EmptySparseMatrixTraits{
  private:
    typedef typename ELEMENT_MATRIX_INTERFACE::DataDriver ElementDataDriver;
    static const int level=ElementDataDriver::level+1;

    typedef Legolas::MatrixShape<level> Data;
    
    typedef Legolas::InputMatrixOptions<Legolas::Actual> MatrixOptions;

    typedef Legolas::EmptySparseMatrixDefinition<ElementDataDriver> BaseDefinition;
    
    struct MatrixDefinition : public BaseDefinition{
      typedef Legolas::MatrixShape<level> Data;
      typedef typename BaseDefinition::GetElement GetElement;
      
      typedef typename BaseDefinition::template FilledRows<Data> FilledRows;
      typedef typename BaseDefinition::template FilledCols<Data> FilledCols;
      
      static inline GetElement sparseGetElement(int i , int j, const Data & data) { return GetElement();}
    };
  public:
    typedef Legolas::GenericMatrixInterface<MatrixDefinition,MatrixOptions,ELEMENT_MATRIX_INTERFACE> MatrixInterface;
  };

  template <class ELEMENT_MATRIX_INTERFACE>
  class EmptySparseMatrixInterface : public EmptySparseMatrixTraits<ELEMENT_MATRIX_INTERFACE>::MatrixInterface
  {
  };
};

#endif
    
    
    
