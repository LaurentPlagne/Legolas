/**
 * project  DESCARTES
 *
 * @file     DRGenericMatrix.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRGENERICMATRIX_HXX__
#define __LEGOLAS_DRGENERICMATRIX_HXX__

#include "Legolas/Matrix/MatrixStructures/DRMatrix/DRGenericMatrixInterfaceTraits.hxx"

namespace Legolas{

  template<class D_GENERIC_MATRIX_INTERFACE, class R_GENERIC_MATRIX_INTERFACE>
  class DRGenericMatrixInterface:public DRGenericMatrixInterfaceTraits<D_GENERIC_MATRIX_INTERFACE,
								       R_GENERIC_MATRIX_INTERFACE>::MatrixInterface
  {
  private:
  
    typedef typename DRGenericMatrixInterfaceTraits<D_GENERIC_MATRIX_INTERFACE, 
						    R_GENERIC_MATRIX_INTERFACE>::MatrixInterface BaseClass;
  public:

    typedef typename BaseClass::Matrix            Matrix;
    typedef typename BaseClass::Copy              Copy;
    typedef typename BaseClass::View              View;
    typedef typename BaseClass::ConstView         ConstView;
  
  };


}

#endif
