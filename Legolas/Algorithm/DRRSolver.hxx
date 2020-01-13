#ifndef __LEGOLAS_DRRSOLVER_HXX__
#define __LEGOLAS_DRRSOLVER_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"

namespace Legolas{

  class DRRSolver{

  public :

    static int _max_iter;
    static double _epsilon;

    template <class DRMATRIX, class RMATRIX, class VECTOR, class VECTOR_INOUT> 
    static inline void apply(const DRMATRIX & DR, 
			     const RMATRIX & R,
			     const VECTOR  & B,
			     VECTOR_INOUT  & X)
    {

      IterationControl<VECTOR> iter(_max_iter,_epsilon);

      typename VECTOR::Element s(B[0]);

      typename DRMATRIX::DMatrixInterface::ConstView d(DR.D());
      typename DRMATRIX::RMatrixInterface::ConstView r(DR.R());

      int size=B.size();
    
      do {
       
	for (int i = 0 ; i < size ; i++ ){
	 
	  s = B[i];	
	 
	  for ( int j = i+1 ; j < size ; j++ ){
	   
	    s-=r(i,j)*X[j];
	    s-=R(i,j)*X[j];
	   
	  }

	  for ( int j = 0 ; j < i ; j++ ){
	  
	    s-=r(i,j)*X[j];
	    s-=R(i,j)*X[j];
	   
	  }

	  X[i]=s/d(i,i);


	}

      } while(!iter.end(X));
   

      return ;
    }

  };

  // Static variables initialization

  int DRRSolver::_max_iter=30;
  double DRRSolver::_epsilon=1.e-5;

}

#endif	
