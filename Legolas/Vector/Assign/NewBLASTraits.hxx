#ifndef __LEGOLAS_NEWBLASTRAITS_HXX__
#define __LEGOLAS_NEWBLASTRAITS_HXX__

#ifdef LEGOLAS_WITH_BLAS

extern "C"
{
#include "Legolas/Common/cblas.h"
}


#endif


namespace Legolas{

  struct NewBLASTraits{

    template <class INDEX, class REAL_TYPE, class SOURCE, class TARGET>
    static inline void axpy(INDEX begin, INDEX end, const REAL_TYPE & factor,const SOURCE & source, TARGET & target){
      for (INDEX i=begin; i < end ; ++i) target[i]+=factor*source[i];
    }
    
    template <class INDEX, class SOURCE, class TARGET>
    static inline void copy(INDEX begin, INDEX end, const SOURCE & source, TARGET & target){
      for (INDEX i=begin; i < end ; ++i) target[i]=source[i];
    }
    
    template <class INDEX, class TARGET, class REAL_TYPE>
    static inline void scal(INDEX begin, INDEX end, const REAL_TYPE factor, TARGET & target){
      for (INDEX i=begin; i < end ; ++i) target[i]*=factor;
    }
    
    template <class INDEX, class LEFT, class RIGHT>
    static inline double dot(INDEX begin, INDEX end, const LEFT & left, const RIGHT &right){
      double result=0.0;
      for (INDEX i=begin; i < end ; ++i) result+=left[i]*right[i];
      return result;
    }
    
  
    template <class INDEX, class REAL_TYPE, class SOURCE, class TARGET>
    static inline void axpy(INDEX N, const REAL_TYPE & factor,const SOURCE & source, TARGET & target){
      NewBLASTraits::axpy(0,N,factor,source,target);
    }
    
    template <class INDEX, class SOURCE, class TARGET>
    static inline void copy(INDEX N, const SOURCE & source, TARGET & target){
      NewBLASTraits::copy(0,N,source,target);
    }
    
    template <class INDEX, class TARGET, class REAL_TYPE>
    static inline void scal(INDEX N, const REAL_TYPE factor, TARGET & target){
      NewBLASTraits::scal(0,N,factor,target);
    }
    
    template <class INDEX, class LEFT, class RIGHT>
    static inline double dot(INDEX N, const LEFT & left, const RIGHT &right){
      return NewBLASTraits::dot(0,N,left,right);
    }

#ifdef LEGOLAS_WITH_BLAS
  
    // double  
    static inline void axpy(int N, const double & factor,const double * source, double * target){
      MESSAGE("BLAS daxpy called from LEGOLAS");
      cblas_daxpy(N,factor,source,1,target,1);
    }
    static inline void copy(int N, const double * source, double * target){
      INFOS("BLAS dcopy called from LEGOLAS");
      cblas_dcopy(N,source,1,target,1);
    }
    static inline void scal(int N, const double & factor, double * target){
      MESSAGE("BLAS dscal called from LEGOLAS");
      cblas_dscal(N,factor,target,1);
    }

    static inline double dot(int N,const double * left, const double * right){
      MESSAGE("BLAS ddot called from LEGOLAS");
      return cblas_ddot(N,left,1,right,1);
    }
    
    // float
    static inline void axpy(int N, const float & factor,const float * source, float * target){
      MESSAGE("BLAS saxpy called from LEGOLAS");
      cblas_saxpy(N,factor,source,1,target,1);
    }
    static inline void copy(int N, const float * source, float * target){
      MESSAGE("BLAS scopy called from LEGOLAS");
      cblas_scopy(N,source,1,target,1);
    }
    static inline void scal(int N, const float & factor, float * target){
      MESSAGE("BLAS sscal called from LEGOLAS");
      cblas_sscal(N,factor,target,1);
    }
    static inline double dot(int N,const float * left, const float * right){
      MESSAGE("BLAS sdot called from LEGOLAS");
      double result=cblas_sdot(N,left,1,right,1);
      return result;
    }
#endif //LEGOLAS_WITH_BLAS
  };

}
#endif //__LEGOLAS_NEWBLASTRAITS_HXX__



// #include <cstring>
// #include "emmintrin.h" 
// #include <string>

// namespace Legolas{

//   template <class REAL_TYPE>
//   struct NewBLASTraits{
    
//     static inline void setValue(const int N, const REAL_TYPE value, REAL_TYPE * target){
//       for (int i=0; i < N ; ++i) target[i]=value;
//     }

//     static inline void addValue(const int N, const REAL_TYPE value, REAL_TYPE * target){
//       for (int i=0; i < N ; ++i) target[i]+=value;
//     }


//     static inline void minusValue(const int N, const REAL_TYPE value, REAL_TYPE * target){
//       for (int i=0; i < N ; ++i) target[i]-=value;
//     }

    
//     static inline void axpy(const int N, const REAL_TYPE & factor,const REAL_TYPE * source, REAL_TYPE * target){
//       for (int i=0; i < N ; ++i) target[i]+=factor*source[i];
//     }
//     static inline void copy(const int N, const REAL_TYPE * source, REAL_TYPE * target){
//       const int copyLength=N*sizeof(REAL_TYPE);
//       std::memcpy(target,source,copyLength);
//       //      for (int i=0; i < N ; ++i) target[i]=source[i];
//     }
//     static inline void scal(const int N, const REAL_TYPE & factor, REAL_TYPE * target){
//       for (int i=0; i < N ; ++i) target[i]*=factor;
//     }


//     static inline double dot(const int N,const REAL_TYPE * left, const REAL_TYPE * right){
//       double result=0.0;
//       for (int i=0; i < N ; ++i) result+=left[i]*right[i];
//       return result;
//     }

//     static inline double squareNorm2(const int N,const REAL_TYPE * vec){
//       double result=0.0;
//       for (int i=0; i < N ; ++i){
// 	const double vi=vec[i];
// 	result+=vi*vi;
//       }
//       return result;
//     }

//     static inline float recursive_dot(const int N,const REAL_TYPE * left, const REAL_TYPE * right){

//       if (N>100){
// 	const int ns2=N/2;
// 	return dot(ns2,left,right)+dot(N-ns2,left+ns2,right+ns2);
//       }
//       else{
// 	float result=0.0;	
// 	for (int i=0; i < N ; ++i) result+=left[i]*right[i];
// 	return result;
//       }
      
//     }

//   };
// }

// #ifdef LEGOLAS_WITH_BLAS

// extern "C"
// {
// #include "Legolas/Common/cblas.h"
// }

// namespace Legolas{

//   template <>
//   struct NewBLASTraits<double>{
    
//    static inline void setValue(const int N, const double value, double * target){
//       for (int i=0; i < N ; ++i) target[i]=value;
//     }


//     static inline void addValue(const int N, const double value, double * target){
//       for (int i=0; i < N ; ++i) target[i]+=value;
//     }


//     static inline void minusValue(const int N, const double value, double * target){
//       for (int i=0; i < N ; ++i) target[i]-=value;
//     }
    


//     static inline void axpy(const int N, const double & factor,const double * source, double * target){
//       MESSAGE("BLAS daxpy called from LEGOLAS");
//       cblas_daxpy(N,factor,source,1,target,1);
//     }

// //     static inline void copy(const int N, const double * source, double * target){
// //       //      MESSAGE("target="<<target);
// //       //      MESSAGE("source="<<source);
// //       for(int i = 0 ; i < N ; i+=2){
// // 	__m128d tmp=_mm_load_pd(source+i);
// // 	_mm_stream_pd(target+i, tmp);
// // 	//_mm_storeu_pd(target+i, tmp);
// //       }
// //       //      MESSAGE("ICI");
// //     }
//     static inline void copy(const int N, const double * source, double * target){
//       MESSAGE("BLAS dcopy called from LEGOLAS");
//       cblas_dcopy(N,source,1,target,1);
//     }

//     static inline void scal(const int N, const double & factor, double * target){
//       MESSAGE("BLAS dscal called from LEGOLAS");
//       cblas_dscal(N,factor,target,1);
//     }

//     static inline double dot(const int N,const double * left, const double * right){
//       MESSAGE("BLAS ddot called from LEGOLAS");
//       return cblas_ddot(N,left,1,right,1);
//     }
//   };
  
//   template <>
//   struct NewBLASTraits<float>{

    
//     static inline void setValue(const int N, const float value, float * target){
//       for (int i=0; i < N ; ++i) target[i]=value;
//     }

//     static inline void addValue(const int N, const float value, float * target){
//       for (int i=0; i < N ; ++i) target[i]+=value;
//     }


//     static inline void minusValue(const int N, const float value, float * target){
//       for (int i=0; i < N ; ++i) target[i]-=value;
//     }

//     static inline void axpy(const int N, const float & factor,const float * source, float * target){
//       if (N>LARGE_ENOUGH){
// 	MESSAGE("BLAS saxpy called from LEGOLAS");
// 	cblas_saxpy(N,factor,source,1,target,1);
//       }
//       else{
// 	for (int i=0; i < N ; ++i) target[i]+=factor*source[i];
//       }
//     }
//     static inline void copy(const int N, const float * source, float * target){
//       if (N>LARGE_ENOUGH){
// 	MESSAGE("BLAS scopy called from LEGOLAS");
// 	cblas_scopy(N,source,1,target,1);
//       }
//       else{
// 	for (int i=0; i < N ; ++i) target[i]=source[i];
//       }
//     }
//     static inline void scal(const int N, const float & factor, float * target){
//       if (N>LARGE_ENOUGH){
// 	MESSAGE("BLAS sscal called from LEGOLAS");
// 	cblas_sscal(N,factor,target,1);
//       }
//       else{
// 	for (int i=0; i < N ; ++i) target[i]*=factor;
//       }
//     }
//     static inline double dot(const int N,const float * left, const float * right){
//       double result=0.0;
//       if (N>LARGE_ENOUGH){
// 	MESSAGE("BLAS sdot called from LEGOLAS");
// 	result=cblas_sdot(N,left,1,right,1);
//       }
//       else{
// 	for (int i=0; i < N ; ++i) result+=left[i]*right[i];
//       }
//       return result;
//     }
//   };
// }

// #endif // LEGOLAS_WITH_BLAS



// #endif
