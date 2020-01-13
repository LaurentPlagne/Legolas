#ifndef __LEGOLAS_VECTOREXPRESSIONTRAITS_HXX__
#define __LEGOLAS_VECTOREXPRESSIONTRAITS_HXX__

#include "Legolas/Vector/BaseVector.hxx"

namespace Legolas{


  template <class SUB_EXPR, class BINARY_OP,class ELEMENT>
  struct BinaryVectorExpressionTraits{

    typedef SUB_EXPR ReturnType;

    template <class LEFT, class RIGHT>
    static ReturnType getElement(const LEFT & left,const RIGHT & right, SizeType i){
      return ReturnType(left[i],right[i]);
    }
  };

  template <class SUB_EXPR,class BINARY_OP>
  struct BinaryVectorExpressionTraits<SUB_EXPR,BINARY_OP,double>
  {
    typedef double ReturnType;

    template <class LEFT, class RIGHT>
    static inline double getElement(const LEFT & left,const RIGHT & right, SizeType i){
      return BINARY_OP::template Engine<double>::apply(left[i],right[i]);
    }
  };

  
  template <class SUB_EXPR,class BINARY_OP>
  struct BinaryVectorExpressionTraits<SUB_EXPR,BINARY_OP,float>{

    typedef float ReturnType;

    template <class LEFT, class RIGHT>
    static inline float getElement(const LEFT & left,const RIGHT & right, SizeType i){
      return BINARY_OP::template Engine<float>::apply(left[i],right[i]);
    }
  };

  /*****************************************************************************/

  template <class SUB_EXPR,class ELEMENT>
  struct UnaryVectorExpressionTraits{
      
    typedef SUB_EXPR ReturnType;

    template <class VECTOR, class REAL_TYPE>
    static inline ReturnType getElement(const REAL_TYPE & factor,const VECTOR & v, const SizeType & i){
      return ReturnType(v[i],factor);
    }
    // 	if (v[i].isAllocated()){
    // 	  return factor*v[i];
    // 	}
    // 	else{
    // 	  return v[i];
    // 	}
    //       }
  };

  template <class SUB_EXPR>
  struct UnaryVectorExpressionTraits<SUB_EXPR,double>{
    
    typedef double ReturnType;

    template <class VECTOR, class REAL_TYPE>
    static inline double getElement(const REAL_TYPE & factor,const VECTOR & v, const SizeType & i){
      return factor*v[i];
    }
  };


  template <class SUB_EXPR>
  struct UnaryVectorExpressionTraits<SUB_EXPR,float>{

    typedef float ReturnType;
    
    template <class VECTOR, class REAL_TYPE>
    static inline float getElement(const REAL_TYPE & factor,const VECTOR & v, const SizeType & i){
      return factor*v[i];
    }
  };



}

#endif
