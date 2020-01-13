/**
 * project  DESCARTES
 *
 * @file     BandMatrixInversionByGaussSeidelAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#if ! defined( __BANDMATRIXINVERSIONBYGAUSSSEIDELALGORITHM__HXX__ )
#define __LEGOLAS_BANDMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"

namespace Legolas{

  class BandMatrixInversionByGaussSeidelAlgorithm{
  public :

    static int _max_iter;
    static double _epsilon;
    
    template <class BANDED_MATRIX,class VECTOR, class VECTOR_INOUT > 
    static inline void apply(const BANDED_MATRIX & A, 
			     const VECTOR & B,
			     VECTOR_INOUT & X)
    {
      int max_iter=10000000;
      double epsilon=1.e-4;
    
      IterationControl<VECTOR> iter(_max_iter,_epsilon);

      VECTOR_INOUT Xold(X);
      typename VECTOR::Element s(X[0]);

      int size=B.size();

      int linf=A.linf();
      int lsup=A.lsup();

    
      do {
      
	Xold=X;
      
	for (int i=0 ; i < size ; i++ ){
	
	  s=B[i];	

	  int jmax=min(i+lsup+1,size);

	  for (int j=i+1 ; j < jmax ; j++ ){
	  
	    s-=A.bandedGetElement(i,j)*X[j];
	  
	  }
	
	  int jmin=max(i-linf,0);

	  for (int j=jmin ; j < i ; j++ ){
	  
	    s-=A.bandedGetElement(i,j)*X[j];
	  
	  }
	
	  X[i]=s/A(i,i);
		
	}
	
	// residual calculation

	//R=A*X-B;
	Xold-=X;
      
      }while(!iter.end(Xold));
   
      //    return X;
    }

  };

  // Static variables initialization

  int BandMatrixInversionByGaussSeidelAlgorithm::_max_iter=1000000;
  double BandMatrixInversionByGaussSeidelAlgorithm::_epsilon=1.e-5;


}

#endif		/* #if ! defined( __GaussSeidel__HXX__ ) */
