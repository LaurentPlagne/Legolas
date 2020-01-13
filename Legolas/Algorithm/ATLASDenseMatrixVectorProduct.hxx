/**
 * project  DESCARTES
 *
 * @file     ATLASDenseMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ATLASDENSEMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_ATLASDENSEMATRIXVECTORPRODUCT_HXX__
#include  <vector>
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"


#ifdef GLASS_USE_ATLAS

namespace Legolas{

#include "UTILITES.hxx"
#include "Legolas/Algorithm/CBlasGemvTraits.hxx"


  class ATLASDenseMatrixVectorProduct{
  public:

    template <class ASSIGN_MODE>
    class Engine{
    public:
      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const MATRIX & A , 
			       const VECTOR & X ,
			       VECTOR_INOUT & Y)
      {

	int N=X.size();

	typedef typename MATRIX::RealType RealType;

	const RealType * AData = A.getDataReference().matrix().getPointer();
	const RealType * XData = X.getDataReference().getPointer();

	RealType * YData = Y.getDataReference().getPointer();

	RealType a=ASSIGN_MODE::alpha();
	RealType b=ASSIGN_MODE::beta();

	gemvTraits<RealType>::apply(CblasRowMajor,CblasNoTrans,N,N,a,AData,N,XData,1,b,YData,1);
      
      }

    };

    class Transpose{
    public:
      template <class ASSIGN_MODE>
      class Engine{
      public:
	template <class MATRIX,class VECTOR, class VECTOR_INOUT>
	static inline void apply(const MATRIX & A , 
				 const VECTOR & X ,
				 VECTOR_INOUT & Y)
	{
	 
	  int N=X.size();
	 
	  typedef typename MATRIX::RealType RealType;
	 
	  const RealType * AData = A.getDataReference().matrix().getPointer();
	  const RealType * XData = X.getDataReference().getPointer();
	 
	  RealType * YData = Y.getDataReference().getPointer();
	 
	  RealType a=ASSIGN_MODE::alpha();
	  RealType b=ASSIGN_MODE::beta();
	 
	  gemvTraits<RealType>::apply(CblasRowMajor,CblasTrans,N,N,a,AData,N,XData,1,b,YData,1);
	 
	}
       
      };

    };
  
    
  };

}

#else
namespace Legolas{
  typedef SparseMatrixVectorProduct ATLASDenseMatrixVectorProduct;
}

#endif // GLASS_USE_ATLAS



#endif
