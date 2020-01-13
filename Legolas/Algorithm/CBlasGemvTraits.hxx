/**
 * project  DESCARTES
 *
 * @file     CBLAS_gemvTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */

#ifndef __LEGOLAS_CBLASGEMVTRAITS_HXX__
#define __LEGOLAS_CBLASGEMVTRAITS_HXX__


extern "C"
{
#include <atlas_level1.h>
#include <atlas_level2.h>
#include <atlas_level3.h>
#include "cblas.h"

}

namespace Legolas{

  template < class REAL_TYPE >
  class gemvTraits{
  public:
    void apply(const int Order,
	       const int TransA, const int M, const int N,
	       const REAL_TYPE alpha, const REAL_TYPE *A, const int lda,
	       const REAL_TYPE *X, const int incX, const REAL_TYPE beta,
	       REAL_TYPE *Y, const int incY){
      INFOS("WRONG TYPE");
      throw std::runtime_error("WRONG TYPE");
    }
  };

  template <>
  class gemvTraits<double>{
  public:
    static inline void apply(const enum CBLAS_ORDER Order,
			     const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
			     const double alpha, const double *A, const int lda,
			     const double *X, const int incX, const double beta,
			     double *Y, const int incY){
      cblas_dgemv(Order,TransA,M,N,alpha,A,lda,X,incX,beta,Y,incY);
    }
  };

  template <>
  class gemvTraits<float>{
  public:
    static inline void apply(const enum CBLAS_ORDER Order,
			     const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
			     const float alpha, const float *A, const int lda,
			     const float *X, const int incX, const float beta,
			     float *Y, const int incY){
      cblas_sgemv(Order,TransA,M,N,alpha,A,lda,X,incX,beta,Y,incY);
    }
  };


}

#endif  
  
