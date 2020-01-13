/**
 * project  DESCARTES
 *
 * @file     QuadMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   2009
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - 2009
 */
#ifndef __LEGOLAS_QUADMATRIXINTERFACE_HXX__
#define __LEGOLAS_QUADMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/MatrixStructures/QuadMatrix/QuadMatrix.hxx"
#include "Legolas/Algorithm/QuadMatrixInversionByGaussSeidelAlgorithm.hxx"
#include "Legolas/Algorithm/QuadMatrixVectorProduct.hxx"

namespace Legolas{

  template<class MI_00, class MI_01, class MI_10, class MI_11,
	   class QUAD_INV=QuadMatrixInversionByGaussSeidelAlgorithm,
	   class QUAD_MULT=QuadMatrixVectorProduct>
  class QuadMatrixInterface{
  public:
    typedef QuadMatrix<MI_00,MI_01,MI_10,MI_11,QUAD_INV,QUAD_MULT> Matrix;
    
    typedef typename Matrix::DataDriver  DataDriver;
    typedef Matrix   Copy;
    typedef Matrix & View;
    typedef const Matrix & ConstView;
  };
}

#endif
