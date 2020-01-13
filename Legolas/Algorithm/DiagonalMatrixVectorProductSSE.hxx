/**
 * project  DESCARTES
 *
 * @file     DiagonalMatrixVectorProductSSE.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DIAGONALMATRIXVECTORPRODUCTSSE_HXX__
#define __LEGOLAS_DIAGONALMATRIXVECTORPRODUCTSSE_HXX__

#include "UTILITES.hxx"
#include "X86Timer.hxx"

//#define TIME_DMVP

namespace Legolas{

  // Y=A*X

  class DiagonalMatrixVectorProductSSE{
  public:

    template<class ASSIGN_MODE>
    class Engine{
    public:


      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const MATRIX & A , 
			       const VECTOR & X ,
			       VECTOR_INOUT & Y){
	
	//	X86Timer chronos;

	
	typedef typename MATRIX::RealType RealType;
	const Legolas::Vector<RealType> & diag=A.getRef().diagonal();
	ASSIGN_MODE::apply(Y,diag*X);

	//	ASSIGN_MODE::applyOnViews(Y.eigenVectorView(),diag.eigenVectorView()*X.eigenVectorView());

	// const int nrows=A.nrows();	
	// for (int i=0 ; i < nrows ; ++i){
	//   ASSIGN_MODE::apply(Y[i],A.diagonalGetElement(i)*X[i]);
	// }

	return ;
      }
    };
    
    class Transpose{
    public:
      template<class ASSIGN_MODE>
      class Engine{
      public:
	template <class MATRIX,class VECTOR, class VECTOR_INOUT>
	static inline void apply(const MATRIX & A , 
				 const VECTOR & X ,
				 VECTOR_INOUT & Y){
	  typedef typename MATRIX::RealType RealType;
	  const Legolas::Vector<RealType> & diag=A.getRef().diagonal();
	  ASSIGN_MODE::apply(Y,diag*X);


	  return ;

	}
      };
    };
  };
}
#endif
