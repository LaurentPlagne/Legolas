#ifndef __LEGOLAS_VECTOROPERATIONS_HXX__
#define __LEGOLAS_VECTOROPERATIONS_HXX__


#include <iostream>
#include <cmath>

#include "Legolas/Vector/Expression/VectorExpression.hxx"
#include "Legolas/Vector/BaseVector.hxx"


namespace Legolas{
  
  struct Add{
    
    template <class RETURN_TYPE>
    struct Engine{
      template <class LEFT, class RIGHT>
      static inline RETURN_TYPE apply(const LEFT & left,const  RIGHT & right){
	return left+right;
	//	RETURN_TYPE result=left+right;
	//	turn result;
      }
    };

    static inline double  factor( void ) {return 1.0;}

    template <class T>
    static inline T apply(T left, T right){
      T result=left+right;
      return result;
    }
   
#ifdef LEGOLAS_USE_EIGEN
    template <class T1, class T2>
    static auto autoApply(T1 left,T2 right) -> decltype(left+right)
    {
      return left+right;
    }
#endif
    
  };
  
  struct Minus{

    template <class RETURN_TYPE>
    struct Engine{
      template <class LEFT, class RIGHT>
      static inline RETURN_TYPE apply(const LEFT & left,const  RIGHT & right){
	return left-right;
	//	RETURN_TYPE result=left-right;
	//	return result;
      }
    };

    static inline double  factor( void ) {return -1.0;}

    template <class T>
    static inline T apply(T left, T right){
      T result=left-right;
      return result;
    }

#ifdef LEGOLAS_USE_EIGEN
    template <class T1, class T2>
    static inline auto autoApply(const T1 & left,const T2 & right) -> decltype(left-right)
    {
      return left-right;
    }
#endif

  };


  struct Multiply{

    template <class RETURN_TYPE>
    struct Engine{
      template <class LEFT, class RIGHT>
      static inline RETURN_TYPE apply(const LEFT & left,const  RIGHT & right){
	return left*right;
	//	RETURN_TYPE result=left-right;
	//	return result;
      }
    };

    static inline double  factor( void ) {return 1.0;}

    template <class T>
    static inline T apply(T left, T right){
      T result=left*right;
      return result;
    }

#ifdef LEGOLAS_USE_EIGEN
    template <class T1, class T2>
    static inline auto autoApply(const T1 & left,const T2 & right) -> decltype(left*right)
    {
      return left*right;
    }
#endif
  };
  
  template <class L, class R >
  VectorExpression< L, Add, R > operator + (const Legolas::BaseVector<L> & left, 
					    const Legolas::BaseVector<R> & right){
    return VectorExpression<L,Add,R >(left,right);
  }


  template <class L, class R >
  VectorExpression< L, Multiply, R > operator * (const Legolas::BaseVector<L> & left, 
						 const Legolas::BaseVector<R> & right){
    return VectorExpression<L,Multiply,R >(left,right);
  }


  template <class L, class R >
  VectorExpression< L, Minus, R > operator - (const BaseVector<L> & left, 
					      const BaseVector<R> & right){
    return VectorExpression<L,Minus,R >(left,right);
  }

  template <class L>
  VectorScalarExpression<L,double> operator * (const BaseVector<L> & v,
					       const double & a){
    return VectorScalarExpression<L,double>(v,a);
  }

  template <class L>
  VectorScalarExpression<L,float> operator * (const BaseVector<L> & v,
					      const float & a){
    return VectorScalarExpression<L,float>(v,a);
  }
  
  template <class R>
  VectorScalarExpression<R,double> operator * (const double & a,
					       const BaseVector<R> & v){
    return VectorScalarExpression<R,double>(v,a);
  }

  template <class R>
  VectorScalarExpression<R,float> operator * (const float & a,
					      const BaseVector<R> & v){
    return VectorScalarExpression<R,float>(v,a);
  }

  inline const double & transpose(const double & factor){
    return factor;
  }

  inline const float & transpose(const float & factor){
    return factor;
  }

  template <class FUNCTOR,class R>
  VectorMapExpression<FUNCTOR,R> map(const FUNCTOR & functor,
				     const BaseVector<R> & v){
    return VectorMapExpression<FUNCTOR,R>(functor,v);
  }



//   inline double dot(const double & l, const double & r){
//     return l*r;
//   }
//   inline double dot(const float & l, const float & r){
//     return l*r;
//   }

  
//   template <class L, class R>
//   inline double dot(const BaseVector<L> & left, const BaseVector<R> & right){
//     const L & l=left.getCVR();
//     const R & r=right.getCVR();

//     double result=0.0;
    
//     //    INFOS("ici");

//     if (l.isAllocated() && r.isAllocated()){

//       const SizeType n=l.size();
//       for (SizeType i=0 ; i < n ; i++){
// 	result+=dot(l[i],r[i]);
// 	//	result=dot(r[i],l[i])+result;
//       }
      
//     }
//     return result;
//   }


  inline void dumpSizes(const double & l, const int level=0){
    return;
  }

  inline void dumpSizes(const float & l, const int level=0){
    return;
  }
  
//   template <class TOTO>
//   inline void dumpSizes(const TOTO & left, const int level=0){  
//     std::cout << "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"<<std::endl;
//     return;
//   }


  template <class L>
  inline void dumpSizes(const BaseVector<L> & left, const int level=0){
    const L & l=left.getCVR();
    
    for (int i=0 ; i<level ; ++i) std::cout << "\t" ;
    std::cout << "size="<<l.size();
    if (l.isAllocated()){
      std::cout << std::endl;
      if (!L::isVector1D()){
	const SizeType n=l.size();
	for (SizeType i=0 ; i < n ; i++){
	  dumpSizes(l[i],level+1);
	}
      }
    }
    else{
      std::cout << " (not allocated)" << std::endl;;
    }
    return ;
  }
  

  inline double squareNorm2(const double & l){
    return l*l;
  }

  inline float squareNorm2(const float & l){
    return l*l;
  }


//   template <class DERIVED>
//   inline double squareNorm2(const BaseVector<DERIVED> & vec){
//     const DERIVED & v=vec.getCVR();
//     double result=0.0;
//     if (v.isAllocated()){
//       const SizeType n=v.size();

//       for (SizeType i=0 ; i < n ; i++){
// 	result+=squareNorm2(v[i]);
//       }
//     }
//     return result;
//   }

  template <class DERIVED>
  inline double norm2(const BaseVector<DERIVED> & vec){
    return sqrt(squareNorm2(vec));
  }


  inline void inject(const double & source,
	      double & target){
    target=source;
  }

  inline void inject(const float & source,
	      float & target){
    target=source;
  }

  template <class DERIVED>
  inline void inject(const BaseVector<DERIVED> & source,
		     BaseVector<DERIVED> & target){
    target.getVR().inject(source.getCVR());
  }
	 


  template <class VECTOR>
  typename VECTOR::Shape getShape(const VECTOR & v) { return v.shape() ;}

  inline int getShape(const float & v) { return 0 ;}  
  inline int getShape(const double & v) { return 0 ;}  

  //  typename VECTOR::Shape getShape(const VECTOR & v) { return v.shape() ;}
  


 
}



#endif
