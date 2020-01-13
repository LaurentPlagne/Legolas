/**
 * project  DESCARTES
 *
 * @file     SparseDRMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPARSEDRMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_SPARSEDRMATRIXVECTORPRODUCT_HXX__

namespace Legolas{

  //A=D+R => AX=DX+RX

  class SparseDRMatrixVectorProduct{
  public :

    template <class ASSIGN_MODE>
    class Engine{
    public:
      template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const DR_MATRIX & A, 
			       const VECTOR & X,
			       VECTOR_INOUT & Y)
      {

	//      INFOS("Passage dans SparseDRMatrixVectorProduct") ;
	if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }

	typedef typename DR_MATRIX::DMatrixInterface DMatrixInterface;
	typedef typename DR_MATRIX::RMatrixInterface RMatrixInterface;
	typedef typename RMatrixInterface::Matrix::FilledRows FilledRows;
      
	typename DMatrixInterface::ConstView D(A.D());
	typename RMatrixInterface::ConstView R(A.R());
      
	ASSIGN_MODE::initialize(Y);
	FilledRows rows(R);

	for (int i=0; i< rows.end() ; rows.next(i)){

	  ASSIGN_MODE::apply(Y[i],D.diagonalGetElement(i)*X[i]);
	  //	s=D.diagonalGetElement(i)*X[i];

	  typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	
	  for (int j=cols.begin() ; j< cols.end() ; cols.next(j)){

	    if (i!=j){
	      ASSIGN_MODE::accumulate(Y[i],R.sparseGetElement(i,j)*X[j]);
	      //	    s+=R.sparseGetElement(i,j)*X[j];
	    }
	  
	  }

	  //	ASSIGN_MODE::apply(Y[i],s);
	
	}
      
	return ;
      }
    };

    // 

    class Transpose{
    public:
      template <class ASSIGN_MODE>
      class Engine{
      public:
	template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
	static inline void apply(const DR_MATRIX & A, 
				 const VECTOR & X,
				 VECTOR_INOUT & Y)
	{
	
	  //      INFOS("Passage dans SparseDRMatrixVectorProduct") ;
	  if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
	
	  typedef typename DR_MATRIX::DMatrixInterface DMatrixInterface;
	  typedef typename DR_MATRIX::RMatrixInterface RMatrixInterface;
	  typedef typename RMatrixInterface::Matrix::FilledRows FilledRows;
	
	  typename DMatrixInterface::ConstView D(A.D());
	  typename RMatrixInterface::ConstView R(A.R());
	
	  ASSIGN_MODE::initialize(Y);
	  FilledRows rows(R);
	
	  for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	  
	    ASSIGN_MODE::accumulate(Y[i],transpose(D.diagonalGetElement(i))*X[i]);
	  
	    typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	  
	    for (int j=cols.begin() ; j< cols.end() ; cols.next(j)){
	    
	      if (i!=j){
		ASSIGN_MODE::accumulate(Y[j],transpose(R.sparseGetElement(i,j))*X[i]);
	      }
	    
	    }
	  
	  }
	
	  return ;
	}
      };
    };
  };

}

#endif	
