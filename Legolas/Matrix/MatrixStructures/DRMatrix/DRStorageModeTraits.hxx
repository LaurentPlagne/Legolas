/**
 * project  DESCARTES
 *
 * @file     DRStorageModeTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRSTORAGEMODETRAITS_HXX__
#define __LEGOLAS_DRSTORAGEMODETRAITS_HXX__

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/ActualDRMatrixInterface.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/VirtualDRMatrixInterface.hxx"

namespace Legolas{

  template < class DR_MATRIX_DEFINITION, class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE, class DR_INV, class DR_MULT, class STORAGE_MODE>
  struct DRStorageModeTraits{
    typedef VoidObject Type;
  };

  template < class DR_MATRIX_DEFINITION, class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE,class DR_INV, class DR_MULT>
  struct DRStorageModeTraits<DR_MATRIX_DEFINITION,D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT,Virtual>{
    typedef VirtualDRMatrixInterface<DR_MATRIX_DEFINITION,D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT> Type;
  };


  template < class DR_MATRIX_DEFINITION, class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE,class DR_INV, class DR_MULT>
  struct DRStorageModeTraits<DR_MATRIX_DEFINITION,D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT,Actual>{
    typedef ActualDRMatrixInterface<DR_MATRIX_DEFINITION,D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT> Type;
  };

}

#endif


