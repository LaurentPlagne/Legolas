/**
 * project  DESCARTES
 *
 * @file     GenericMatrixInterfaceTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_GENERICMATRIXINTERFACETRAITS_HXX__
#define __LEGOLAS_GENERICMATRIXINTERFACETRAITS_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/Helper/Default.hxx"
#include "Legolas/Matrix/VirtualMatrix/VirtualMatrixInterface.hxx"
#include "Legolas/Matrix/ActualMatrix/ActualFromVirtualMatrixInterface.hxx"
#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
#include "Legolas/Matrix/StorageModeTraits/StorageModeInterfaceTraits.hxx"
#include "Legolas/Matrix/MatrixOptions/DefaultMatrixOptions.hxx"

namespace Legolas{

  template <class MATRIX_DEFINITION,
	    class MATRIX_OPTIONS=Default,
	    class MATRIX_ELEMENT_INTERFACE=Default>
  class GenericMatrixInterfaceTraits{
  public:

    

    typedef MATRIX_DEFINITION                                                               MatrixDefinition;
    typedef DefaultMatrixOptions<typename MatrixDefinition::MatrixStructure,MATRIX_OPTIONS> MatrixOptions; 
  private:
    typedef MATRIX_ELEMENT_INTERFACE              MatrixElementInterface;
    // Virtual or Actual
    typedef typename MatrixOptions::StorageMode   StorageMode;

    // MatrixInterface calculation 
    typedef  VirtualMatrixInterface<MatrixDefinition,MatrixOptions,MatrixElementInterface>            VMI;
    typedef  ActualFromVirtualMatrixInterface<MatrixDefinition,MatrixOptions,MatrixElementInterface>  AMI;
  public:
    typedef typename StorageModeInterfaceTraits<VMI,AMI,StorageMode>::Type MatrixInterface;
  private:
    //    typedef typename MatrixInterface::Matrix                               Matrix;
  
  };


}

#endif
