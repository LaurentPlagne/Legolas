/**
 * project  DESCARTES
 *
 * @file     QuadMatrixInversionByGaussSeidelAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_QUADMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__
#define __LEGOLAS_QUADMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "Legolas/Algorithm/TriDiagonalMatrixInversionByLUDecomposition.hxx"

#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"
#include "Legolas/Algorithm/ComputeLinearSystemResidual.hxx"

namespace Legolas{

  class QuadMatrixInversionByGaussSeidelAlgorithm{
  public :
  
    template <class QUAD_MATRIX,class VECTOR, class VECTOR_INOUT > 
    class Engine{
    public:
    

      static inline void apply(const QUAD_MATRIX & A, 
			       const VECTOR & B,
			       VECTOR_INOUT & X)
      {
	
	typename VECTOR::Element s(B[0]);
	
	typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<QUAD_MATRIX,VECTOR,VECTOR_INOUT> > IterationControl;

	X86Timer    chronometre;

	IterationControl iter(A,B,X);
	
	do {

	  s=B[0];
	  s-=A.M01()*X[1];
	  X[0]=s/A.M00();

	  s=B[1];
	  s-=A.M10()*X[0];
	  X[1]=s/A.M11();

	}while(!iter.end(X));
	
	ComputeLinearSystemResidual::apply(A,X,B);
      
	return ;
      }
    };
    
    class Transpose{
    public:
      template <class ASSIGN_MODE>
      class Engine{
      public:

	template <class QUAD_MATRIX,class VECTOR, class VECTOR_INOUT > 
	static inline void apply(const QUAD_MATRIX & A, 
				 const VECTOR & B,
				 VECTOR_INOUT & X)
	{
	  INFOS("not yet implemented");
	  throw std::runtime_error("Not yet implemented");
	};
      };
    };

  };

}

#endif
