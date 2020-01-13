#ifndef __LEGOLAS_MNMATRIXITERATIVEINVERSIONALGORITHM_HXX__
#define __LEGOLAS_MNMATRIXITERATIVEINVERSIONALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"

namespace Legolas{

  class MNMatrixIterativeInversionAlgorithm{

  public :

    static int _max_iter;
    static double _epsilon;

    template <class MN_MATRIX,class VECTOR, class VECTOR_INOUT> 
    static inline void apply(const MN_MATRIX & A, 
			     const VECTOR    & B,
			     VECTOR_INOUT    & X)
    {

      IterationControl<VECTOR> iter(_max_iter,_epsilon);

      typename MN_MATRIX::MMatrixInterface::ConstView M(A.M());
      typename MN_MATRIX::NMatrixInterface::ConstView N(A.N());
    
      VECTOR S(B);
      VECTOR T(B);

      do {

	S = B ;

	T = N*X ;

	S -= T ;

	X = S / M ;

      } while(!iter.end(X));
   

      return ;
    }

  };

  // Static variables initialization

  int MNMatrixIterativeInversionAlgorithm::_max_iter=1000000;
  double MNMatrixIterativeInversionAlgorithm::_epsilon=1.e-5;

}

#endif	
