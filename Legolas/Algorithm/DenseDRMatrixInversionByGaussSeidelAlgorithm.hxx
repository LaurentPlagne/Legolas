/**
 * project  DESCARTES
 *
 * @file     DenseDRMatrixInversionByGaussSeidelAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DENSEDRMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__
#define __LEGOLAS_DENSEDRMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__

#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"

namespace Legolas{

  class DenseDRMatrixInversionByGaussSeidelAlgorithm{

  public :

    template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
    static inline void apply(const DR_MATRIX & A, 
			     const VECTOR & B,
			     VECTOR_INOUT & X)
    {

      DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<DR_MATRIX,VECTOR,VECTOR_INOUT> > iter(A,B,X);

      typename VECTOR::Element s(B[0]);

      int size=B.size();

      typename DR_MATRIX::DMatrixInterface::ConstView D(A.D());
      typename DR_MATRIX::RMatrixInterface::ConstView R(A.R());

      bool thisIsTheEnd=false;

      do {

	for (int i=0 ; i < size ; i++ ){

	  s=B[i];	

	  for (int j=i+1 ; j < size ; j++ ){
	  
	    s-=R(i,j)*X[j];
	  	  
	  }

	  for (int j=0 ; j < i ; j++ ){
	  
	    s-=R(i,j)*X[j];
	  
	  }

	  X[i]=s/D(i,i);


	}

	thisIsTheEnd=iter.end(X);
	
      }while(!thisIsTheEnd);

   
      return ;
    }

  };

}

#endif	
