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
#ifndef __LEGOLAS_PRODUCTCOMPOUNDMATRIXINTERFACE_HXX__
#define __LEGOLAS_PRODUCTCOMPOUNDMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/MatrixStructures/ProductCompound/ProductCompoundMatrix.hxx"
#include "Legolas/Algorithm/ProductCompoundMatrixVectorProduct.hxx"
#include "Legolas/Algorithm/ProductCompoundMatrixInversion.hxx"


namespace Legolas{
  
  template<class L_GENERIC_MATRIX_INTERFACE, 
	   class R_GENERIC_MATRIX_INTERFACE, 
	   class PC_INV=ProductCompoundMatrixInversion,
	   class PC_MULT=ProductCompoundMatrixVectorProduct>
  class ProductCompoundMatrixInterface{
  public:
    typedef ProductCompoundMatrix<L_GENERIC_MATRIX_INTERFACE,R_GENERIC_MATRIX_INTERFACE,PC_INV,PC_MULT> Matrix;
    
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
