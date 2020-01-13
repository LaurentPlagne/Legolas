#ifndef __LEGOLAS_POWERALGORITHM_HXX__
#define __LEGOLAS_POWERALGORITHM_HXX__

// New Power Algorithm : This algorithm solves the Generalized Eigenvalue Problem
//                   find the biggest in modulus lambda such as :  Ax=(1/lambda)Bx.
// It works whatever the sign of its value.
//

#include "Legolas/Algorithm/NoInversePowerAcceleration.hxx"

namespace Legolas{

  class PowerAlgorithm
  {
  public:
    template <template <class> class ITERATION_CONTROL, template <class, class, class> class ACCELERATION_METHOD=NoInversePowerAcceleration> 
    class Engine
    {
			
    public:
      template <class MATRIX_INVERSIBLE, class MATRIX, class VECTOR, class SOURCE>
      static inline void apply(const MATRIX_INVERSIBLE & A,  
			       const MATRIX & B, 
			       typename VECTOR::RealType & lambda, 
			       VECTOR & X,
			       SOURCE & S,
			       const DK::Number & maxExternalIteration,
			       const DK::Number & maxNonAcceleratedIteration,
			       const DK::Number & maxAcceleratedIteration,
			       const typename VECTOR::RealType & fluxExternalPrecision,
			       const typename VECTOR::RealType & eigenvaluePrecision,
			       const DK::Number & verbose,
			       )
      {
	RealType                             dotXX, dotXXnew; //P. G. NEW : dotXXnew for the dotXX normalization (used in iter.end)
	VECTOR                                             Xold(X);
	typedef ACCELERATION_METHOD< MATRIX_INVERSIBLE,MATRIX,VECTOR >    Acceleration;
	typedef ITERATION_CONTROL< Acceleration >                         IterationControl;

	IterationControl    iter(A,B,lambda,X,
				 maxExternalIteration,
				 maxNonAcceleratedIteration,
				 maxAcceleratedIteration,
				 fluxExternalPrecision,
				 eigenvaluePrecision,
				 verbose);

	do 
	  {
	    INFOS("ICI"),
	    //
	    // X = A^{-1}.B.Xold
	    //
	    Xold = X;
	    S = B * X;
	    //
	    // The following line is usefull as X is used as the initial guess of
	    // the iterative resolution of AX=S
	    //
	    X *= lambda;
	    //
	    X = S / A;
	    //
	    // lambda = (X,X) / (X,Xold) 
	    //        = (H.Xold,H.Xold) / (H.Xold,Xold) avec H = A^{-1}B
	    //
	    dotXX = dot(X,X);
	    lambda = dotXX / dot(Xold,X);
	    //
	    // scaling de X afin de ne pas diverger  alors qu'il y a convergence
	    //
	    X /= lambda;
            dotXXnew = dotXX / (lambda * lambda); // P. G. NEW : used in iter.end

	    INFOS("ICI"),

	  } while( !iter.end(X,lambda,Xold,dotXXnew) ) ; //P. G. NEW : dotXX replaced by dotXXnew

#ifdef GLASS_COMPUTE_RESIDUAL
	//
	// Test . For any non zero vector Y
	// lambda=<BX|Y>/<AX|Y>
	//    
	VECTOR    AX(X);
	//
	//   1 : build A*X
	//
	AX = A * X;
	//
	//   2 : build B*X
	//
	Xold = B * X;
	//
	//   3 : build a test vector Y
	//
	VECTOR    Y(X);
	Y = 1.0;
	//
	//   4 : check lambda
	//
	RealType    newLambda = dot(Xold,Y) / dot(AX,Y);
	INFOS( "newLambda=" << newLambda << "  newLambda - lambda = "<< newLambda - lambda );
#endif
      }
    };


  };

}

#endif		
