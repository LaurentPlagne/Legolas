/**
 * project  DESCARTES
 *
 * @file     DRGenericMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRGENERICMATRIXINTERFACE_HXX__
#define __LEGOLAS_DRGENERICMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/MatrixStructures/DRMatrix/DRGenericMatrixInterfaceTraits.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/DRMatrix.hxx"
#include "Legolas/Algorithm/SparseDRMatrixInversionByGaussSeidelAlgorithm.hxx"
#include "Legolas/Algorithm/SparseDRMatrixVectorProduct.hxx"

namespace Legolas{
  

#define NEW
#ifdef NEW

  template<class D_GENERIC_MATRIX_INTERFACE, 
	   class R_GENERIC_MATRIX_INTERFACE, 
	   class DR_INV=SparseDRMatrixInversionByGaussSeidelAlgorithm,
	   class DR_MULT=SparseDRMatrixVectorProduct>
  class DRGenericMatrixInterface{
  public:
    typedef DRMatrix<D_GENERIC_MATRIX_INTERFACE,R_GENERIC_MATRIX_INTERFACE,DR_INV,DR_MULT> Matrix;
    
    typedef typename Matrix::DataDriver  DataDriver;
    typedef Matrix   Copy;
    typedef Matrix & View;
    typedef const Matrix & ConstView;
  };
}
#else



  template<class D_GENERIC_MATRIX_INTERFACE, 
	   class R_GENERIC_MATRIX_INTERFACE, 
	   class DR_INV=SparseDRMatrixInversionByGaussSeidelAlgorithm,
	   class DR_MULT=SparseDRMatrixVectorProduct>
  class DRGenericMatrixInterface:public DRGenericMatrixInterfaceTraits<D_GENERIC_MATRIX_INTERFACE,
								       R_GENERIC_MATRIX_INTERFACE,
								       DR_INV,
								       DR_MULT>::MatrixInterface
  {
  private:
  
    typedef DRGenericMatrixInterfaceTraits<D_GENERIC_MATRIX_INTERFACE, 
					   R_GENERIC_MATRIX_INTERFACE,
					   DR_INV,
					   DR_MULT> DRGMIT;

    typedef typename DRGMIT::MatrixInterface BaseClass;

  public:

    typedef typename DRGMIT::Data                 Data;
    typedef typename BaseClass::Matrix            Matrix;
    typedef typename BaseClass::Copy              Copy;
    typedef typename BaseClass::View              View;
    typedef typename BaseClass::ConstView         ConstView;
  
  };
}

#endif

#endif
