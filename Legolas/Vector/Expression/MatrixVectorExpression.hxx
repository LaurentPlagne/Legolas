/**
 * project  DESCARTES
 *
 * @file     MatrixVectorExpression.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_MATRIXVECTOREXPRESSION_HXX__
#define __LEGOLAS_MATRIXVECTOREXPRESSION_HXX__

#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"

namespace Legolas{

  template < class MATRIX, class OPERATOR, class VECTOR >
  class MatrixVectorExpression: public BaseVector< MatrixVectorExpression<MATRIX,OPERATOR,VECTOR> >{
  public:

    typedef typename MATRIX::StoreType MatrixType;
    typedef typename VECTOR::StoreType VectorType;

    typedef typename VECTOR::Shape Shape;
    
//     MatrixVectorExpression(const BaseMatrix<MATRIX> & matrixOperandInit , 
// 			   const BaseVector<VECTOR> & vectorOperandInit):_matrixOperand(matrixOperandInit.getCMR()),
// 									 _vectorOperand(vectorOperandInit.getCVR())
//     {};

    MatrixVectorExpression(const MATRIX & matrixOperandInit , 
			   const VECTOR & vectorOperandInit,
			   const Shape & shape):_matrixOperand(matrixOperandInit),
						_vectorOperand(vectorOperandInit),
						_shape(shape)
    {};


    MatrixType  matrixOperand( void ) const {return _matrixOperand ;} ;
    VectorType  vectorOperand( void ) const {return _vectorOperand ;} ;

    inline Shape shape( void ) const { return _shape;}

    //Let's try this....
    //    typedef typename VECTOR::Shape Shape;
    //    inline Shape shape( void ) const { return _matrixOperand.getRowShape() ;}

    
  private:

    MatrixType  _matrixOperand ;
    VectorType  _vectorOperand ;
    Shape _shape;
  };


  class AnyMatrixVectorMultExpression{
  public:
    
    AnyMatrixVectorMultExpression(const AnyMatrix & matrixOperand , 
				  const AnyVector & vectorOperand):_matrixOperand(matrixOperand),
								   _vectorOperand(vectorOperand)
    {
    };
    
    const AnyMatrix & matrixOperand( void ) const {return _matrixOperand ;}
    const AnyVector & vectorOperand( void ) const {return _vectorOperand ;}
    
  private:

    const AnyMatrix &  _matrixOperand ;
    const AnyVector &  _vectorOperand ;

  };

  class AnyMatrixVectorInverseExpression{
  public:
    
    AnyMatrixVectorInverseExpression(const AnyMatrix & matrixOperand , 
				     const AnyVector & vectorOperand):_matrixOperand(matrixOperand),
								      _vectorOperand(vectorOperand)
    {
    };
    
    const AnyMatrix & matrixOperand( void ) const {return _matrixOperand ;}
    const AnyVector & vectorOperand( void ) const {return _vectorOperand ;}
    
  private:

    const AnyMatrix &  _matrixOperand ;
    const AnyVector &  _vectorOperand ;

  };

  /////////////////////////////////////////////////////////////////////////////

  class TransposeAnyMatrix{
  public:
    TransposeAnyMatrix(const AnyMatrix & anyMatrix):matrixOperand_(anyMatrix){}
    const AnyMatrix & matrixOperand( void ) const{ return matrixOperand_ ;}
  private:
    const AnyMatrix & matrixOperand_;
  };


  class TransposeAnyMatrixVectorMultExpression{
  public:
    
    TransposeAnyMatrixVectorMultExpression(const AnyMatrix & matrixOperand , 
					   const AnyVector & vectorOperand):_matrixOperand(matrixOperand),
									    _vectorOperand(vectorOperand)
    {
    };
    
    const AnyMatrix & matrixOperand( void ) const {return _matrixOperand ;}
    const AnyVector & vectorOperand( void ) const {return _vectorOperand ;}
    
  private:

    const AnyMatrix &  _matrixOperand ;
    const AnyVector &  _vectorOperand ;
    
  };


  class TransposeAnyMatrixVectorInverseExpression{
  public:
    
    TransposeAnyMatrixVectorInverseExpression(const AnyMatrix  & matrixOperand , 
					      const AnyVector & vectorOperand):_matrixOperand(matrixOperand),
									       _vectorOperand(vectorOperand)
    {
    };
    
    const AnyMatrix & matrixOperand( void ) const {return _matrixOperand ;}
    const AnyVector & vectorOperand( void ) const {return _vectorOperand ;}
    
  private:

    const AnyMatrix &  _matrixOperand ;
    const AnyVector &  _vectorOperand ;

  };

}

#endif
