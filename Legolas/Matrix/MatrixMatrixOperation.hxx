/**
 * project  DESCARTES
 *
 * @file     MatrixVectorOperations.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_MATRIXVECTOROPERATIONS_HXX__
#define __LEGOLAS_MATRIXVECTOROPERATIONS_HXX__

#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/MatrixVectorExpression.hxx"

#include "Legolas/Matrix/MatrixStructures/Sparse/RealSparseMatrix.hxx"
#include "DisplayLatex.hxx"
#include "DisplayHTML.hxx"

#include <string>
#include <fstream>

namespace Legolas{

  /*---------------------------- X/A ----------------------------------*/

  template <class MATRIX,class VECTOR>
  void
  divide(const BaseVector<VECTOR> & vectorOperand,
	 const BaseMatrix<MATRIX> & matrixOperand)
  {
    INFOS("divide");
  };
  

  template <class MATRIX,class VECTOR>
  MatrixVectorExpression<MATRIX,typename MATRIX::MatrixVectorInverse,VECTOR>
  divide2(const BaseVector<VECTOR> & vectorOperand,
	 const BaseMatrix<MATRIX> & matrixOperand)
    
  {
    INFOS("divide2");
    return MatrixVectorExpression<MATRIX,typename MATRIX::MatrixVectorInverse,VECTOR>(matrixOperand,vectorOperand);
  };


  template <class MATRIX,class VECTOR>
  MatrixVectorExpression<MATRIX,
			 typename MATRIX::MatrixVectorInverse,
			 VECTOR >
  operator /(const BaseVector<VECTOR> & vectorOperand,
	     const BaseMatrix<MATRIX> & matrixOperand)

  {
    return MatrixVectorExpression<MATRIX,
      typename MATRIX::MatrixVectorInverse,
      VECTOR >(matrixOperand,vectorOperand);
  };

  /*---------------------------- A*X ----------------------------------*/

  template <class MATRIX,class VECTOR>
  MatrixVectorExpression<MATRIX,
			 typename MATRIX::MatrixVectorProduct,
			 VECTOR >
  operator *(const BaseMatrix<MATRIX> & matrixOperand,
	     const BaseVector<VECTOR> & vectorOperand)

  {
    return MatrixVectorExpression<MATRIX,
      typename MATRIX::MatrixVectorProduct,
      VECTOR >(matrixOperand,vectorOperand);
  };


  /*---------------------------- A*X ----------------------------------*/
  
  inline AnyMatrixVectorMultExpression
  operator *(const AnyMatrix & anyMatrix,
	     const AnyVector & anyVector)
  {
    return AnyMatrixVectorMultExpression(anyMatrix,anyVector);
  };




  /*---------------------------- tranpose(A) ----------------------------------*/


  template <class MATRIX> 
  TransposedBaseMatrix<MATRIX> transpose(const BaseMatrix<MATRIX> & matrix){ 
    return TransposedBaseMatrix<MATRIX>(matrix);
  }

  inline const double & transpose(const double & factor){
    return factor;
  }

  inline const float & transpose(const float & factor){
    return factor;
  }


  /*---------------------------- X/transpose(A) ----------------------------------*/

  template <class MATRIX,class VECTOR>
  MatrixVectorExpression<MATRIX,
			 typename MATRIX::MatrixVectorInverse::Transpose,
			 VECTOR >
  operator /(const BaseVector<VECTOR> & vectorOperand,
	     const TransposedBaseMatrix<MATRIX> & matrixOperand)

  {
    return MatrixVectorExpression<MATRIX,
      typename MATRIX::MatrixVectorInverse::Transpose,
      VECTOR >(matrixOperand.getBaseMatrix(),vectorOperand);
  };

  /*---------------------------- transpose(A)*X ----------------------------------*/

  template <class MATRIX,class VECTOR>
  MatrixVectorExpression<MATRIX,
			 typename MATRIX::MatrixVectorProduct::Transpose,
			 VECTOR >
  operator *(const TransposedBaseMatrix<MATRIX> & matrixOperand,
	     const BaseVector<VECTOR> & vectorOperand)

  {

    return MatrixVectorExpression<MATRIX,
      typename MATRIX::MatrixVectorProduct::Transpose,
      VECTOR >(matrixOperand.getBaseMatrix(),vectorOperand);
  };

}

#endif
