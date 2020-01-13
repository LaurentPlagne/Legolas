/**
 * project  DESCARTES
 *
 * @file     SparseDRMatrixInversionBySORAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPARSEDRMATRIXINVERSIONBYSORALGORITHM_HXX__
#define __LEGOLAS_SPARSEDRMATRIXINVERSIONBYSORALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"

#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"
#include "AssignMode.hxx"

namespace Legolas{

  class SparseDRMatrixInversionBySORAlgorithm{
  public :

    template <class ASSIGN_MODE=EqualAssign<double> >
    class Engine{
    public:
    
      template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const DR_MATRIX & A, 
			       const VECTOR & B,
			       VECTOR_INOUT & X)
      {

	typedef typename DR_MATRIX::DMatrixInterface DMatrixInterface;
	typedef typename DR_MATRIX::RMatrixInterface RMatrixInterface;
	typedef typename RMatrixInterface::Matrix::FilledRows FilledRows;
      
	typename DMatrixInterface::ConstView D(A.D());
	typename RMatrixInterface::ConstView R(A.R());

	typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<DR_MATRIX,VECTOR,VECTOR_INOUT> > IterationControl;

	if (D.nrows()==1){
	  X[0]=B[0]/D.diagonalGetElement(0);
	}
	else{
	
	  IterationControl iter(A,B,X);
	  typename VECTOR::Element s(B[0]);
	  int size=B.size();
	  FilledRows rows(R);

	  VECTOR_INOUT XP(X);
	  VECTOR_INOUT E(X);
	  VECTOR_INOUT C(X);

	  do {

	    E=A*X;
	    E-=B;

	    // C=(L+D)^{-1} E
	    for (int i=0 ; i < size ; i++){
	      typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	      s=E[i];
	      for (int j=cols.begin() ; j < i ; cols.next(j) ){
		s-=R.sparseGetElement(i,j)*C[j];
	      }
	      C[i]=s/D.diagonalGetElement(i);
	    }

	    if (iter.getCurrentIteration()>500){
	      double omega=1.607;
	      C*=omega;
	    }


	    // Correction addition
	    X-=C;

	  
	  }while(!iter.end(X));
	
	}
      
	return ;
      }

    };

  };

}

#endif
