/**
 * project  DESCARTES
 *
 * @file     StorageModeInterfaceTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_STORAGEMODEINTERFACETRAITS_HXX__
#define __LEGOLAS_STORAGEMODEINTERFACETRAITS_HXX__

#include "Legolas/Matrix/Helper/VoidObject.hxx"
#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/ActualDRMatrixInterface.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/VirtualDRMatrixInterface.hxx"

namespace Legolas{

  template <class VIRTUAL_INTERFACE, class ACTUAL_INTERFACE, class STORAGE_MODE>
  struct StorageModeInterfaceTraits{
    typedef VoidObject Type;
  };

  template <class VIRTUAL_INTERFACE, class ACTUAL_INTERFACE>
  struct StorageModeInterfaceTraits<VIRTUAL_INTERFACE,ACTUAL_INTERFACE,Actual>{
    typedef ACTUAL_INTERFACE Type;
  };

  template <class VIRTUAL_INTERFACE, class ACTUAL_INTERFACE>
  struct StorageModeInterfaceTraits<VIRTUAL_INTERFACE,ACTUAL_INTERFACE,Virtual>{
    typedef VIRTUAL_INTERFACE Type;
  };

}

#endif
