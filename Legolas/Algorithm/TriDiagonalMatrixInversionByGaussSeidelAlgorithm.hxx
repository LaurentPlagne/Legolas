/**
 * project  DESCARTES
 *
 * @file     TriDiagonalMatrixInversionByGaussSeidelAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#if ! defined( __TRIDIAGONALMATRIXINVERSIONBYGAUSSSEIDELALGORITHM__HXX__ )
#define __LEGOLAS_TRIDIAGONALMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"

namespace Legolas{

  class TriDiagonalMatrixInversionByGaussSeidelAlgorithm{
  public :

    template <class ASSIGN_MODE>
    class Engine{
    public:
      template <class TRIDIAGONAL_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const TRIDIAGONAL_MATRIX & A, 
			       const VECTOR & B,
			       VECTOR_INOUT & X)
      {
	typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<TRIDIAGONAL_MATRIX,VECTOR,VECTOR_INOUT> > IterationControl;
	IterationControl iter(A,B,X);
    
	typename VECTOR::Element s(B[0]);

	int size=B.size();

	do {
      
	  // first row :
	
	  s=B[0]-A.upperDiagonalGetElement(0)*X[1];

	  X[0]=s/A.diagonalGetElement(0);

	  // intermediate rows

	  for (int i=1 ; i < size-1 ; i++ ){
	
	    s=B[i]-A.upperDiagonalGetElement(i)*X[i+1]-A.lowerDiagonalGetElement(i)*X[i-1];	
	
	    X[i]=s/A.diagonalGetElement(i);
		
	  }

	  // last row :

	  s=B[size-1]-A.lowerDiagonalGetElement(size-1)*X[size-2];

	  X[size-1]=s/A.diagonalGetElement(size-1);

	
	}while(!iter.end(X));
      }
    };
  };

}

#endif		/* #if ! defined( __GaussSeidel__HXX__ ) */
