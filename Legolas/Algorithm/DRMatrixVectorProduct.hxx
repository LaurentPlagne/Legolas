/**
 * project  DESCARTES
 *
 * @file     DRMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#if ! defined( __DRMATRIXVECTORPRODUCT__HXX__ )
#define __LEGOLAS_DRMATRIXVECTORPRODUCT_HXX__

namespace Legolas{

  // A=D+R => AX=DX+RX

  class DRMatrixVectorProduct{
  public :

    static int _max_iter;
    static double _epsilon;

    template <class ASSIGN_MODE>
    class Engine{
    public:
      template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const DR_MATRIX & A, 
			       const VECTOR & X,
			       VECTOR_INOUT & Y)
      {
	if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }

	INFOS("Passage dans DRMatrixVectorProduct") ;
    throw std::runtime_error("Passage dans DRMatrixVectorProduct");
      
	typedef typename DR_MATRIX::DMatrixInterface DMatrixInterface;
	typedef typename DR_MATRIX::RMatrixInterface RMatrixInterface;
      
	typename DMatrixInterface::ConstView D(A.D());
	typename RMatrixInterface::ConstView R(A.R());
      
	typename VECTOR_INOUT::Element s(Y[0]);
      
	int rRows=R.nrows();
	int rCols=R.ncols();
      
	INFOS("rRows="<<rRows);
      
	for (int i=0; i< rRows ; i++){
	
	  s=D.diagonalGetElement(i)*X[i];
	
	  INFOS("rCols="<<rCols);
	
	  for (int j=0; j< rCols ; j++){
	  
	    INFOS("j="<<j);
	  
	    s+=R(i,j)*X[j];
	  
	  }

	  ASSIGN_MODE::apply(Y[i],s);
	
	}
      
	return ;
      }
    
    };
  };


} 
#endif		/* #if ! defined( __DRMatrixVectorProduct__HXX__ ) */
