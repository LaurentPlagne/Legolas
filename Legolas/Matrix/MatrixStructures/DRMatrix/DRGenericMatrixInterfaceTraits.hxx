/**
 * project  DESCARTES
 *
 * @file     DRGenericMatrixInterfaceTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRGENERICMATRIXINTERFACETRAITS_HXX__
#define __LEGOLAS_DRGENERICMATRIXINTERFACETRAITS_HXX__

#include "Legolas/Matrix/MatrixStructures/DRMatrix/DRMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/DRStorageModeTraits.hxx"

namespace Legolas{

  template<class D_GENERIC_MATRIX_INTERFACE, class R_GENERIC_MATRIX_INTERFACE, class DR_INV, class DR_MULT>
  class DRGenericMatrixInterfaceTraits
  {
  private:

    typedef D_GENERIC_MATRIX_INTERFACE DMatrixInterface;
    typedef R_GENERIC_MATRIX_INTERFACE RMatrixInterface;


    typedef typename DMatrixInterface::MatrixDefinition DMatrixDefinition;
    typedef typename RMatrixInterface::MatrixDefinition RMatrixDefinition;


    typedef DRMatrixDefinition<DMatrixDefinition,RMatrixDefinition> DRMI;

    typedef typename DMatrixInterface::MatrixOptions::StorageMode     StorageMode;
  
  public:

    typedef typename DRMI::Data Data;
    typedef typename DRStorageModeTraits<DRMI,DMatrixInterface,RMatrixInterface,DR_INV,DR_MULT,StorageMode>::Type MatrixInterface;
  
  };


}

#endif
