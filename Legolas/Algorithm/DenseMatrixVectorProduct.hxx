/**
 * project  DESCARTES
 *
 * @file     DenseMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DENSEMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_DENSEMATRIXVECTORPRODUCT_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  // Y=A*X

  class DenseMatrixVectorProduct{
  public:
    template <class MATRIX,class VECTOR, class VECTOR_INOUT>
    static inline void apply(const MATRIX & A , 
			     const VECTOR & X ,
			     VECTOR_INOUT & Y)
    {
      // *AP* 15/07/04 Pb à la compilation quand VECTOR != VECTOR_INOUT
      //if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }

      int aRows=A.nrows();
      int aCols=A.ncols();
      INFOS("ICI");
      throw std::runtime_error("ICI");
    
      if ((aRows==1)&&(aCols==1)){
	Y[0]=A(0,0)*X[0];
      }
      else{
      
	typename VECTOR_INOUT::Element s(Y[0]);
      
      
	for (int i=0; i< aRows ; i++){
	
	  s=0.0;
	
	  for (int j=0; j< aCols ; j++){
	  
	    s+=A(i,j)*X[j];
	  
	  }
	
	  Y[i]=s;
	
	}
      
      }
    
      return ;
    }
  
  };




}

#endif
