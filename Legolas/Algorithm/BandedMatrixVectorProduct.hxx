/**
 * project  DESCARTES
 *
 * @file     BandedMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_BANDEDMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_BANDEDMATRIXVECTORPRODUCT_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  class BandedMatrixVectorProduct{
  public:
    template <class BANDED_MATRIX,class VECTOR, class VECTOR_INOUT>
    static inline void apply(const BANDED_MATRIX & A , 
			     const VECTOR & X,
			     VECTOR_INOUT & Y)
    {
      if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }

      typename VECTOR_INOUT::Element s(Y[0]);

      int linf=A.linf(); // Nb of non zero elements below the diagonal
      int lsup=A.lsup(); // Nb of non zero elements above the diagonal
    
      int size=A.nrows();

      for (int i=0; i< size ; i++){
      
	s=0.0;
      
	int jmin=max(i-linf,0);
	int jmax=min(i+lsup+1,size);

	for (int j=jmin; j< jmax ; j++){
	
	  s+=A.bandedGetElement(i,j)*X[j];
	
	}
      
	Y[i]=s;
      
      }
    
      return ;
    }
  
  };
}

#endif
