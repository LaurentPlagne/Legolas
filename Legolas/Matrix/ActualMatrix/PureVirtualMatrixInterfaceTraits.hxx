#ifndef __PURE_VIRTUAL_MATRIX_INTERFACE_TRAITS_HXX__
#define __PURE_VIRTUAL_MATRIX_INTERFACE_TRAITS_HXX__

#include "Legolas/Matrix/ActualMatrix/ActualMatrixInterface.hxx"
#include "Legolas/Matrix/VirtualMatrix/VirtualMatrixInterface.hxx"
#include "Legolas/Matrix/PolymorphicMatrixInterface.hxx"
#include "Legolas/Matrix/MatrixOptions/DefaultMatrixOptions.hxx"


namespace Legolas{

  template <class GENERIC_MATRIX_INTERFACE>
  class PureVirtualMatrixInterfaceTraits{
  private:
    
    typedef typename GENERIC_MATRIX_INTERFACE::MatrixDefinition         MatrixDefinition;
    typedef typename GENERIC_MATRIX_INTERFACE::MatrixOptions            MatrixOptions;
    typedef typename GENERIC_MATRIX_INTERFACE::MatrixElementInterface   MatrixElementInterface;

    typedef DefaultMatrixOptions<typename MatrixDefinition::MatrixStructure,Legolas::InputMatrixOptions<Legolas::Virtual> > PureVirtualMatrixOptions; 

    typedef typename PureVirtualMatrixInterfaceTraits<MatrixElementInterface>::Type PureVirtualMatrixElementInterface;
    
  public:
    typedef VirtualMatrixInterface<MatrixDefinition,PureVirtualMatrixOptions,PureVirtualMatrixElementInterface> Type ;
  };

  template <class REAL_TYPE>
  class PureVirtualMatrixInterfaceTraits< RealElementInterface<REAL_TYPE> >{
  public:
    typedef RealElementInterface<REAL_TYPE> Type;
  };

  template <int LEVEL, class REAL_TYPE>
  class PureVirtualMatrixInterfaceTraits< PolymorphicMatrixInterface<LEVEL,REAL_TYPE> >{
  public:
    typedef PolymorphicMatrixInterface<LEVEL,REAL_TYPE> Type;
  };

}

#endif
