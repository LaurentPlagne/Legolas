/**
 * project  DESCARTES
 *
 * @file     TriDiagonalMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TRIDIAGONALMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_TRIDIAGONALMATRIXVECTORPRODUCT_HXX__
#include "UTILITES.hxx"

namespace Legolas{

  class TriDiagonalMatrixVectorProduct{
  public:

    template<class ASSIGN_MODE>
    class Engine{
    public:
      template <class TRIDIAGONAL_MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const TRIDIAGONAL_MATRIX & A , 
			       const VECTOR & X,
			       VECTOR_INOUT & Y)
      {
	ASSIGN_MODE::initialize(Y);

	int size=A.nrows();
      
	for (int i=0; i< size ; i++){
	  ASSIGN_MODE::accumulate(Y[i],A.diagonalGetElement(i)*X[i]);
	}
      
	for (int i=1; i< size ; i++){
	  ASSIGN_MODE::accumulate(Y[i],A.lowerDiagonalGetElement(i)*X[i-1]);
	}
      
	for (int i=0; i< size-1 ; i++){
	  ASSIGN_MODE::accumulate(Y[i],A.upperDiagonalGetElement(i)*X[i+1]);
	}
      
	return ;
      }
    };
  
  };

}


#endif
