#ifndef __LEGOLAS_LINEARSYSTEMINVERSIONBYITERATIVEMNALGORITHM_HXX__
#define __LEGOLAS_LINEARSYSTEMINVERSIONBYITERATIVEMNALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"

namespace Legolas{



  class LinearSystemInversionByIterativeMNAlgorithm{

  public :

    static int _max_iter;
    static double _epsilon;

    template <class MMATRIX, class NMATRIX, class VECTOR, class VECTOR_INOUT> 
    static inline void apply(const MMATRIX & M, 
			     const NMATRIX & N,
			     const VECTOR  & B,
			     VECTOR_INOUT  & X)
    {

      IterationControl<VECTOR> iter(_max_iter,_epsilon);

      VECTOR S(B);
      VECTOR T(B);

      do {
       

	S = B ;

	T = N*X ;

	S -= T ;
      
	DenseDRMatrixInversionByGaussSeidelAlgorithm::apply(M,S,X) ;

      } while(!iter.end(X));
   

      return ;
    }

  };

  // Static variables initialization

  int LinearSystemInversionByIterativeMNAlgorithm::_max_iter=30;
  double LinearSystemInversionByIterativeMNAlgorithm::_epsilon=1.e-5;

}

#endif	
