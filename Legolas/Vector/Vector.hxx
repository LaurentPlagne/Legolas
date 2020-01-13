#ifndef __LEGOLAS_VECTOR_HXX__
#define __LEGOLAS_VECTOR_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/Shape/ShapeTraits.hxx"
#include "Legolas/Vector/Expression/VectorOperations.hxx"
#include "Legolas/Vector/RectangularMultiVector.hxx"
#include "Legolas/Vector/FlatMultiVector.hxx"

namespace Legolas{
  
#define MultiVector RectangularMultiVector
  //#define MultiVector FlatMultiVector

  template <class REAL_TYPE>
  class Vector : public MultiVector< REAL_TYPE ,1 >{
  public: 
    typedef REAL_TYPE                              RealType;
    typedef MultiVector< REAL_TYPE ,1 >            Base;
    
    Vector( void ):Base(){ MESSAGE("Legolas::Vector Default Ctor");}
    Vector( const Vector & source):Base(source){ MESSAGE("Legolas::Vector Copy Ctor");}
    Vector(const SizeType & size):Base(size){ MESSAGE("Legolas::Vector size Ctor");}

    template <class DERIVED> 
    Vector(const BaseVector<DERIVED> & right):Base(right){MESSAGE("Legolas::Vector BaseVector Ctor");}
  public:
    template <class DERIVED>
    Vector & operator = ( const BaseVector<DERIVED> & right){ 
      this->Base::operator =(right);
      return (*this);
    }
    void operator = (double aReal){ this->Base::operator =(aReal);}
    void operator = (float aReal){ this->Base::operator =(aReal);}

    template<class LEFT, class OPERATOR, class RIGHT>
    void operator = (const MatrixVectorExpression<LEFT,OPERATOR,RIGHT> & expression){
      this->Base::operator = (expression) ;
    }

    template<class T> void operator += (const T & source){this->Base::operator += (source) ;}
    template<class T> void operator -= (const T & source){this->Base::operator -= (source) ;}

    void operator = (const AnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().matrixVectorProduct(expression.vectorOperand(),(*this),0);
    }

    void operator += (const AnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().matrixVectorProduct(expression.vectorOperand(),(*this),1);
    }

    void operator -= (const AnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().matrixVectorProduct(expression.vectorOperand(),(*this),-1);
    }

    void operator = (const AnyMatrixVectorInverseExpression & expression){
      expression.matrixOperand().matrixVectorInverse(expression.vectorOperand(),(*this));
    }

    void operator = (const TransposeAnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().transposeMatrixVectorProduct(expression.vectorOperand(),(*this),0);
    }

    void operator += (const TransposeAnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().transposeMatrixVectorProduct(expression.vectorOperand(),(*this),1);
    }

    void operator -= (const TransposeAnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().transposeMatrixVectorProduct(expression.vectorOperand(),(*this),-1);
    }

  };

}

#endif


  
