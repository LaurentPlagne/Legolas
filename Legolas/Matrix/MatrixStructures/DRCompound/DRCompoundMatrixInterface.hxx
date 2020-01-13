/**
 * project  DESCARTES
 *
 * @file     ProductCompoundMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRCOMPOUNDMATRIXINTERFACE_HXX__
#define __LEGOLAS_DRCOMPOUNDMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/MatrixStructures/DRCompound/DRCompoundMatrix.hxx"
#include "Legolas/Algorithm/DRCompoundMatrixVectorProduct.hxx"
#include "Legolas/Algorithm/DRCompoundMatrixInversion.hxx"


namespace Legolas{
  
  template<class D_GENERIC_MATRIX_INTERFACE, 
	   class R_GENERIC_MATRIX_INTERFACE, 
	   class DRC_INV=DRCompoundMatrixInversion,
	   class DRC_MULT=DRCompoundMatrixVectorProduct>
  class DRCompoundMatrixInterface{
  public:
    typedef DRCompoundMatrix<D_GENERIC_MATRIX_INTERFACE,R_GENERIC_MATRIX_INTERFACE,DRC_INV,DRC_MULT> Matrix;
    
    typedef typename Matrix::DataDriver  DataDriver;
    typedef Matrix   Copy;
    typedef Matrix & View;
    typedef const Matrix & ConstView;

    typedef typename Matrix::MatrixDefinition MatrixDefinition;

    static bool isAMatrix( void ) { return true ; }
    static const bool isAMatrix_=true;

    typedef typename Matrix::Solver Solver;
  };
}

#endif
