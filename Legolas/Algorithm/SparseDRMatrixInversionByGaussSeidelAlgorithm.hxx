/**
 * project  DESCARTES
 *
 * @file     SparseDRMatrixInversionByGaussSeidelAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPARSEDRMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__
#define __LEGOLAS_SPARSEDRMATRIXINVERSIONBYGAUSSSEIDELALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "Legolas/Algorithm/TriDiagonalMatrixInversionByLUDecomposition.hxx"

#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"
#include "Legolas/Algorithm/ComputeLinearSystemResidual.hxx"

namespace Legolas{

  class SparseDRMatrixInversionByGaussSeidelAlgorithm{
  public :
  
    template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
    class Engine :  public LinearSolver<DR_MATRIX,VECTOR_INOUT,VECTOR> {
    public:

      Engine(const DR_MATRIX & A, 
 	     VECTOR_INOUT & X, 
 	     const VECTOR & B):LinearSolver<DR_MATRIX,VECTOR_INOUT,VECTOR>(){}

      
      void transposeSolve(const DR_MATRIX & A, VECTOR_INOUT & X, const VECTOR & B){
	INFOS("not yet implemented");
	throw std::runtime_error("Not yet implemented");
      }

      
      void solve(const DR_MATRIX & A, VECTOR_INOUT & X, const VECTOR & B){
	
	typedef typename DR_MATRIX::DMatrixInterface DMatrixInterface;
	typedef typename DR_MATRIX::RMatrixInterface RMatrixInterface;
	typedef typename RMatrixInterface::Matrix::FilledRows FilledRows;
	
	const typename DMatrixInterface::ConstView D(A.D());
	const typename RMatrixInterface::ConstView R(A.R());
	
	typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<DR_MATRIX,VECTOR,VECTOR_INOUT> > IterationControl;

	X86Timer    chronometre;

	if (D.nrows()==1){
	  X[0]=B[0]/D.diagonalGetElement(0);
	}
	else{
	  
	  IterationControl iter(A,B,X);
	
	  chronometre.start();
	  typename VECTOR::Element s(B[0]);
	  chronometre.stop();
	  chronometre.storeResult("SparseDRMatrixInversionByGaussSeidelAlgorithm typename VECTOR::Element s(B[0])");
	
	  int size=B.size();
	
	  FilledRows rows(R);

	  do {

	    for (int i=0 ; i < size ; i++){

	      chronometre.start();
	      s=B[i];	
	      chronometre.stop();
	      chronometre.storeResult("SparseDRMatrixInversionByGaussSeidelAlgorithm s=B[i];");
	 
	      
	      typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	    
	      for (int j=i+1 ; j < cols.end() ; cols.next(j)){
		s-=R.sparseGetElement(i,j)*X[j];
	      
	      }

	      for (int j=cols.begin() ; j < i ; cols.next(j) ){
		s-=R.sparseGetElement(i,j)*X[j];
	      
	      }

	      X[i]=s/D.diagonalGetElement(i);
	      
	    }
	  
	  }while(!iter.end(X));
	
	}

	//	ComputeLinearSystemResidual::apply(A,X,B);
      
	return ;
      }
    };
    
    class Transpose{
    public:
      template <class ASSIGN_MODE>
      class Engine{
      public:

	template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
	static inline void apply(const DR_MATRIX & A, 
				 const VECTOR & B,
				 VECTOR_INOUT & X)
	{

	  typedef typename DR_MATRIX::DMatrixInterface DMatrixInterface;
	  typedef typename DR_MATRIX::RMatrixInterface RMatrixInterface;
	  typedef typename RMatrixInterface::Matrix::FilledCols FilledCols;
	
	  const typename DMatrixInterface::ConstView D(A.D());
	  const typename RMatrixInterface::ConstView R(A.R());
	
	  typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<DR_MATRIX,VECTOR,VECTOR_INOUT> > IterationControl;	
	
	  if (D.nrows()==1){
	    X[0]=B[0]/transpose(D.diagonalGetElement(0));
	  }
	  else{
	  
	    IterationControl iter(A,B,X);
	    typename VECTOR::Element s(B[0]);

	    int size=B.size();
	
	    FilledCols cols(R);

	    MESSAGE("Transpose Sparse DR GaussSeidel");

	    do {
	      for (int i=size-1 ; i > -1 ; i-- ){
		s=B[i];	
		typename FilledCols::FilledRows rows=cols.filledRowsInCol(i);
		int j=i;
		rows.next(j);
		for ( ; j < rows.end() ; rows.next(j) ) s-=transpose(R.sparseGetElement(j,i))*X[j];
		for (int j=rows.begin() ; j < i ; rows.next(j) )s-=transpose(R.sparseGetElement(j,i))*X[j];
		X[i]=s/transpose(D.diagonalGetElement(i));
	      }
	    }while(!iter.end(X));
	  }
	
	  ComputeLinearSystemResidual::apply(A,X,B,true);

	};
      };
    };

  };

}

#endif
