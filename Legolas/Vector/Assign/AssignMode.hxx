/**
 * project  DESCARTES
 *
 * @file     AssignMode.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ASSIGNMODE_HXX__
#define __LEGOLAS_ASSIGNMODE_HXX__

#include <string>

namespace Legolas{

  class EqualAssign{
  public:

    static std::string name( void ){ return "EqualAssign" ;}

    static inline double alpha ( void ) {return 1.0 ;}
    static inline double beta  ( void ) {return 0.0 ;}

    static inline int tag( void ) {return 0;}

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void apply(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left=right;
    }

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void applyOnViews(GET_ELEMENT left,RIGHT_OPERAND right){
      left=right;
    }

    // template <class GET_ELEMENT, class REAL_TYPE>
    // static inline void applyOnViewsWithScalar(GET_ELEMENT left, const REAL_TYPE &  value){
    //   left.setConstant(value);
    // }

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void accumulate(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left+=right;
    }

    template <class VECTOR>
    static inline void initialize(VECTOR & vector){
      //      typename VECTOR::RealType zero=0.0;
      //      vector=zero;
      vector.nullify();
    }


  };


  class PlusAssign{
  public:

    static std::string name( void ){ return "PlusAssign" ;}

    static inline double alpha ( void ) {return 1.0 ;}
    static inline double beta  ( void ) {return 1.0 ;}

    static inline int tag( void ) {return 1;}

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void apply(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left+=right;
    }

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void applyOnViews(GET_ELEMENT left,RIGHT_OPERAND right){
      left+=right;
    }

    // template <class GET_ELEMENT, class REAL_TYPE>
    // static inline void applyOnViewsWithScalar(GET_ELEMENT left, const REAL_TYPE &  value){
    //   //      left.setConstant(value);
    //   left+=value;
    // }

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void accumulate(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left+=right;
    }


    template <class VECTOR>
    static inline void initialize(const VECTOR & vector){
      return;
    }

  
  };

  class MinusAssign{
  public:

    static std::string name( void ){ return "MinusAssign" ;}

    static inline double alpha ( void ) {return 1.0 ;}
    static inline double beta  ( void ) {return -1.0 ;}

    static inline int tag( void ) {return 2;}

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void apply(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left-=right;
    }

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void applyOnViews(GET_ELEMENT left,RIGHT_OPERAND right){
      left-=right;
    }

    // template <class GET_ELEMENT, class REAL_TYPE>
    // static inline void applyOnViewsWithScalar(GET_ELEMENT left, const REAL_TYPE &  value){
    //   //      left.setConstant(value);
    //   left-=value;
    // }

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void accumulate(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left-=right;
    }

    template <class VECTOR>
    static inline void initialize(const VECTOR & vector){
      return;
    }

  
  };

  class MultAssign{
  public:
    
    static std::string name( void ){ return "MultAssign" ;}
    
    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void applyOnViews(GET_ELEMENT left,RIGHT_OPERAND right){
      left*=right;
    }
    
    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void apply(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left*=right;
    }

    // template <class GET_ELEMENT, class REAL_TYPE>
    // static inline void applyOnViewsWithScalar(GET_ELEMENT left, const REAL_TYPE &  value){
    //   left*=value;
    // }

  };

  class SetAssign{
  public:
    
    static std::string name( void ){ return "SetAssign" ;}
    
    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void applyOnViews(GET_ELEMENT left,RIGHT_OPERAND right){
      left.setConstant(right);
    }

    template <class GET_ELEMENT, class RIGHT_OPERAND>
    static inline void apply(GET_ELEMENT & left,const RIGHT_OPERAND & right){
      left=right;
    }


    
  };


  // class NullAssign{
  // public:
    
  //   static std::string name( void ){ return "SetAssign" ;}
    
  //   template <class GET_ELEMENT, class RIGHT_OPERAND>
  //   static inline void applyOnViews(GET_ELEMENT left,RIGHT_OPERAND right){
  //     left.setZero();
  //   }
    
  // };


}

#endif
		      
		      
		      
