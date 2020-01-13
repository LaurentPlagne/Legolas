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

#include "Legolas/Algorithm/BlockLinearSolverTraits.hxx"

//#include "Legolas/Matrix/MatrixStructures/Sparse/RealSparseMatrix.hxx"
#include "DisplayLatex.hxx"
//#include "DisplayHTML.hxx"
#include "DisplayPNG.hxx"

#include <string>
#include <fstream>

namespace Legolas{

//   /*---------------------------- X/A ----------------------------------*/

//   template <class MATRIX,class VECTOR>
//   void
//   divide(const BaseVector<VECTOR> & vectorOperand,
// 	 const BaseMatrix<MATRIX> & matrixOperand)
//   {
//     INFOS("divide");
//   };
  

//   template <class MATRIX,class VECTOR>
//   MatrixVectorExpression<MATRIX,typename MATRIX::MatrixVectorInverse,VECTOR>
//   divide2(const BaseVector<VECTOR> & vectorOperand,
// 	  const BaseMatrix<MATRIX> & matrixOperand)
    
//   {
//     INFOS("divide2");
//     //    return MatrixVectorExpression<MATRIX,typename MATRIX::MatrixVectorInverse,VECTOR>(matrixOperand,vectorOperand);
//     return MatrixVectorExpression<MATRIX,typename MATRIX::MatrixVectorInverse,VECTOR>(matrixOperand.getCMR(),vectorOperand.getCVR());
//   };


//   template <class MATRIX,class VECTOR>
//   MatrixVectorExpression<MATRIX,
// 				typename MATRIX::MatrixVectorInverse,
// 				VECTOR >
//   operator /(const BaseVector<VECTOR> & vectorOperand,
// 	     const BaseMatrix<MATRIX> & matrixOperand)

//   {
//     return MatrixVectorExpression<MATRIX,
//       typename MATRIX::MatrixVectorInverse,
//       VECTOR >(matrixOperand.getCMR(),vectorOperand.getCVR());//(matrixOperand,vectorOperand);
//   };

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
      VECTOR >(matrixOperand.getCMR(),vectorOperand.getCVR(),matrixOperand.getCMR().getRowShape());//(matrixOperand,vectorOperand);
  };


  /*---------------------------- Any*X ----------------------------------*/
  
  inline AnyMatrixVectorMultExpression
  operator *(const AnyMatrix & anyMatrix,
	     const AnyVector & anyVector)
  {
    return AnyMatrixVectorMultExpression(anyMatrix,anyVector);
  };

  /*---------------------------- X/Any ----------------------------------*/
  
  inline AnyMatrixVectorInverseExpression
  operator /(const AnyVector & anyVector,
	     const AnyMatrix & anyMatrix)
  {
    return AnyMatrixVectorInverseExpression(anyMatrix,anyVector);
  };


  /*---------------------------- T(Any) ----------------------------------*/
  inline TransposeAnyMatrix transpose(const AnyMatrix & anyMatrix){ return TransposeAnyMatrix(anyMatrix);}
  /*---------------------------- T(Any)*X ----------------------------------*/
  
  inline TransposeAnyMatrixVectorMultExpression
  operator *(const TransposeAnyMatrix & transposeAnyMatrix,
	     const AnyVector & anyVector)
  {
    return TransposeAnyMatrixVectorMultExpression(transposeAnyMatrix.matrixOperand(),anyVector);
  };
  /*---------------------------- X/T(Any) ----------------------------------*/
  
  inline TransposeAnyMatrixVectorInverseExpression
  operator /(const AnyVector & anyVector,
	     const TransposeAnyMatrix & transposeAnyMatrix)
  {
    return TransposeAnyMatrixVectorInverseExpression(transposeAnyMatrix.matrixOperand(),anyVector);
  };

  


  /*---------------------------- tranpose(A) ----------------------------------*/


  template <class MATRIX> 
  TransposedBaseMatrix<MATRIX> transpose(const BaseMatrix<MATRIX> & matrix){ 
    return TransposedBaseMatrix<MATRIX>(matrix);
  }


  /*---------------------------- X/transpose(A) ----------------------------------*/

  template <class MATRIX,class VECTOR>
  MatrixVectorExpression<MATRIX,
			 typename MATRIX::MatrixVectorInverse::Transpose,
			 VECTOR >
  operator /(const BaseVector<VECTOR> & vectorOperand,
	     const TransposedBaseMatrix<MATRIX> & matrixOperand)

  {
    assert(matrixOperand.getBaseMatrix().getCMR().getRowShape()==matrixOperand.getBaseMatrix().getCMR().getColShape());
    return MatrixVectorExpression<MATRIX,
      typename MATRIX::MatrixVectorInverse::Transpose,
      VECTOR >(matrixOperand.getBaseMatrix().getCMR(),vectorOperand.getCVR(),matrixOperand.getBaseMatrix().getCMR().getRowShape());//(matrixOperand.getBaseMatrix(),vectorOperand);
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
      VECTOR >(matrixOperand.getBaseMatrix().getCMR(),vectorOperand.getCVR(),matrixOperand.getBaseMatrix().getCMR().getColShape());//(matrixOperand.getBaseMatrix(),vectorOperand);
  };

  template <class TA,class TX, class TB>
  void solve(const TA & A,
	     TX & X,
	     const TB & B)
  {
    //    typename Legolas::BlockLinearSolverTraits<TA>::Type solver(A,X,B);
    typename TA::Solver solver(A,X,B);

    solver.solve(A,X,B);
  };

  template <class TA,class TX, class TB>
  void transposeSolve(const TA & A,
		      TX & X,
		      const TB & B)
  {
    //    typename Legolas::BlockLinearSolverTraits<TA>::Type solver(A,X,B);
    typename TA::Solver solver(A,X,B);

    solver.transposeSolve(A,X,B);
  };



}

#endif
