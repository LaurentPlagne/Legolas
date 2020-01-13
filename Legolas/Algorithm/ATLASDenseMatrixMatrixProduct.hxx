/**
 * project  DESCARTES
 *
 * @file     ATLASDenseMatrixMatrixProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ATLASDENSEMATRIXMATRIXPRODUCT_HXX__
#define __LEGOLAS_ATLASDENSEMATRIXMATRIXPRODUCT_HXX__
#include  <vector>

#include "UTILITES.hxx"

//#define USE_EIGEN

#ifdef USE_EIGEN

#include "Legolas/Algorithm/EigenDenseMatrixMatrixProduct.hxx"
#define ATLASDenseMatrixMatrixProduct EigenDenseMatrixMatrixProduct

#else


extern "C"
{
#include "Legolas/Common/cblas.h"
}

namespace Legolas{

  // C=A*B

  inline void gemm(int M, int N, int K, const double * AData,const double * BData, double * CData){
    cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,M,N,K,1.0,AData,K,BData,N,0.0,CData,N);
  }
  
  inline void gemm(int M, int N, int K, const float * AData,const float * BData, float * CData){
    cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,M,N,K,1.0,AData,K,BData,N,0.0,CData,N);
  }



  class ATLASDenseMatrixMatrixProduct{
  public:

    template <class MATRIX>
    static inline void apply(const MATRIX & A , 
			     const MATRIX & B ,
			     MATRIX & C)
    {
      //C mn = Amk * Bkn

      
      int M=A.nrows();
      int K=A.ncols();
      int N=B.ncols();

//       INFOS("A.nrows()="<<A.nrows());
//       INFOS("A.ncols()="<<A.ncols());

//       INFOS("B.nrows()="<<B.nrows());
//       INFOS("B.ncols()="<<B.ncols());

//       INFOS("C.nrows()="<<C.nrows());
//       INFOS("C.ncols()="<<C.ncols());

      assert(A.ncols()==B.nrows());
      assert(A.nrows()==C.nrows());
      assert(B.ncols()==C.ncols());
    
      typedef typename MATRIX::RealType RealType;
    
      const RealType * AData = A.getDataReference().matrix().getPointer();
      const RealType * BData = B.getDataReference().matrix().getPointer();
      
      RealType * CData = C.getDataReference().matrix().getPointer();

      Legolas::gemm(M,N,K,AData,BData,CData);
    
    }
  
  };

}

#endif

#endif
