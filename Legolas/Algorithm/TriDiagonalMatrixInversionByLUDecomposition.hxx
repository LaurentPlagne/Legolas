/**
 * project  DESCARTES
 *
 * @file     TriDiagonalMatrixInversionByLUDecomposition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#if ! defined( __TRIDIAGONALMATRIXINVERSIONBYLUDECOMPOSITION__HXX__ )
#define __LEGOLAS_TRIDIAGONALMATRIXINVERSIONBYLUDECOMPOSITION_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"

namespace Legolas{

  // LP :See Numerical Recipes in C++ second edition page 54.
  // This algo does not include pivoting, no test for zero pivot (could be added).
  // Probably ok only for simple matrix and vectors (no blocks).
  // For block algo, the matrix matrix product and matrix matrix inversion should be defined...?


  class TriDiagonalMatrixInversionByLUDecomposition{
  public :


    template <class ASSIGN_MODE>
    class Engine{
    public:
    
      template <class TRIDIAGONAL_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const TRIDIAGONAL_MATRIX & A, 
			       const VECTOR & B,
			       VECTOR_INOUT & X)
      {
    
	int size=B.size();

	// workspace :: MatrixElement Vector

	VECTOR S(B);
      
	typename TRIDIAGONAL_MATRIX::MatrixElementInterface::Copy s(A.diagonalGetElement(0));
      
	//	s=A.diagonalGetElement(0);
      
	X[0]=B[0]/s;
      
	for (int i=1 ; i < size ; i++ ){
	
	  S[i]=A.upperDiagonalGetElement(i-1)/s;
	  s=A.diagonalGetElement(i)-A.lowerDiagonalGetElement(i)*S[i];
	  X[i]=(B[i]-A.lowerDiagonalGetElement(i)*X[i-1])/s;
	
	}
      
	for (int i=(size-2) ; i >= 0 ; i-- ){
	
	  X[i]-=S[i+1]*X[i+1];
	
	}
      
      }
    
    };
  };

}

#endif		/* #if ! defined( __TriDiagLU__HXX__ ) */
