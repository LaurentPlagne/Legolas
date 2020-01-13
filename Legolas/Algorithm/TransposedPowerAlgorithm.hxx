#ifndef __LEGOLAS_TRANSPOSEDPOWERALGORITHM_HXX__
#define __LEGOLAS_TRANSPOSEDPOWERALGORITHM_HXX__

// New Power Algorithm : This algorithm solves the Generalized Eigenvalue Problem
//                   find the biggest in modulus lambda such as :  transposed(A)*x=(1/lambda)transposed(B)*x.
// It works whatever the sign of its value.
//

#include "Legolas/Algorithm/NoInversePowerAcceleration.hxx"

namespace Legolas{

  class TransposedPowerAlgorithm
  {
  
  public:
  
    template <template <class> class ITERATION_CONTROL, 
	      template <class, class, class> class ACCELERATION_METHOD=NoInversePowerAcceleration>
    class Engine{

    public:
    
      template <class MATRIX_INVERSIBLE, // X=Y/A exist;
		class MATRIX,            // X=A*Y exist;
		class VECTOR,
		class SOURCE>
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
			       const DK::Number & verbose)
      {
    
	typedef typename VECTOR::RealType RealType;
    
	RealType dotXX, dotXXnew; //P. G. NEW : dotXXnew for the dotXX normalization (used in iter.end)
    
	VECTOR Xold(X);
    
	ITERATION_CONTROL< ACCELERATION_METHOD<MATRIX_INVERSIBLE,MATRIX,VECTOR> > iter(A,B,lambda,X,
										       maxExternalIteration,
										       maxNonAcceleratedIteration,
										       maxAcceleratedIteration,
										       fluxExternalPrecision,
										       eigenvaluePrecision,
										       verbose);

	do {
      
	  Xold=X;
      
	  S=transpose(B)*X;
      
	  X *= lambda;

	  X = S/transpose(A); 
      
	  dotXX= dot(X,X);
	  lambda = dotXX/dot(Xold,X);
	  X /= lambda;
          
          dotXXnew = dotXX / (lambda * lambda); //P. G. NEW : used in iter.end
        } while( !iter.end(X,lambda,Xold,dotXXnew) ) ; //P. G. NEW : dotXX replaced by dotXXnew
    
#ifdef GLASS_COMPUTE_RESIDUAL    

	X*=lambda; // X have been scaled
    
	// Test . For any non zero vector Y
	// lambda=<BtX|Y>/<AtX|Y>
    
	VECTOR AX(X);
    
	//   1 : build At*X
	AX=transpose(A)*X;
    
	//   2 : build Bt*X
	Xold=transpose(B)*X;
    
	//   3 : build a test vector Y
	VECTOR Y(X);
	Y=1.0;
    
	//   4 : check lambda
	RealType newLambda=dot(Xold,Y)/dot(AX,Y);
	INFOS("newLambda="<<newLambda<<"  newLambda-lambda="<<newLambda-lambda);
    
#endif    
      }
    
    };
  };

}

#endif		
