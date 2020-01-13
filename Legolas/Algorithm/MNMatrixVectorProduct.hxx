/**
 * project  DESCARTES
 *
 * @file     MNMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_MNMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_MNMATRIXVECTORPRODUCT_HXX__

namespace Legolas{

  // A=M-N => AX=MX-NX
  // Il n'y a pas d'hypothèse sur le profil de la matrice

  class MNMatrixVectorProduct{

  public :

    template <class MN_MATRIX,class VECTOR, class VECTOR_INOUT > 
    static inline void apply(const MN_MATRIX & A, 
			     const VECTOR    & X,
			     VECTOR_INOUT    & Y)
    {
      if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }

      typedef typename MN_MATRIX::MMatrixInterface MMatrixInterface;
      typedef typename MN_MATRIX::NMatrixInterface NMatrixInterface;

      typename MMatrixInterface::ConstView M(A.M());
      typename NMatrixInterface::ConstView N(A.N());

      typename VECTOR_INOUT::Element s(Y[0]);

      int rRows=N.nrows();
      int rCols=N.ncols();

      //INFOS("rRows="<<rRows);
    
      for (int i=0; i< rRows ; i++){
      
	//INFOS("rCols="<<rCols);

	for (int j=0; j< rCols ; j++){

	  //INFOS("j="<<j);
	  s =  M(i,j)*X[i] ;
	  s -= N(i,j)*X[j] ;
	
	}
      
	Y[i]=s;
      
      }
    
      return ;
    }
  };


}

#endif	
