#ifndef __LEGOLAS_TRANSPOSEDPOWERALGORITHMFORPOSITIVEEIGENVALUE_HXX__
#define __LEGOLAS_TRANSPOSEDPOWERALGORITHMFORPOSITIVEEIGENVALUE_HXX__

// New Power Algorithm : This algorithm solves the Generalized Eigenvalue Problem
//                   find the biggest in modulus lambda such as :  transposed(A)*x=(1/lambda)transposed(B)*x when this value is positive.

#include "Legolas/Algorithm/NoInversePowerAcceleration.hxx"

namespace Legolas{

  class TransposedPowerAlgorithmForPositiveEigenvalue
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
			       const DK::Number & verbose
			       )
      {
	typedef typename VECTOR::RealType    RealType;
	RealType                             dotSS, dotSSold;
	VECTOR                      Sold(S);
	
	typedef ACCELERATION_METHOD< MATRIX_INVERSIBLE,MATRIX,VECTOR >    Acceleration;
	typedef ITERATION_CONTROL< Acceleration >                         IterationControl;

	IterationControl    iter(A,B,lambda,X,
				 maxExternalIteration,
				 maxNonAcceleratedIteration,
				 maxAcceleratedIteration,
				 fluxExternalPrecision,
				 eigenvaluePrecision,
				 verbose);



	X86Timer    chronometre;
	bool finished=false;

	S = transpose(B) * X;
        X*=lambda; // The current have to be multiplied by the eigenvalue in order to have a good initialisation of the iterative Gauss-Seildel process
	dotSSold = Legolas::dot(S,S);

	do
	  {
	    chronometre.start();
	    X = S / transpose(A);
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo X =S/transpose(A)");

	    chronometre.start();
	    Sold=S;
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo Sold=S");


	    chronometre.start();
	    S = transpose(B) * X; // = lambda*Sn+1
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo S =transpose(B)*X");

	    chronometre.start();
	    dotSS=Legolas::dot(S,S); // lambda*lambda*Sn1
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo <S,S>");
						
	    lambda = sqrt(dotSS/dotSSold);
						
	    chronometre.start();
	    S *= (1.0/lambda); // = Sn+1
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo S/=lambda");

	    dotSSold=dotSS/(lambda*lambda);

	    chronometre.start();
	    finished=iter.end(S,lambda,Sold,dotSSold); //P. G. new : dotSSold au lieu de dotSS
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo iter.end(S,lambda,Sold,dotSS)");
						
	    
	  } while( !finished ) ;
        //
        // it is done in order to leave the program with (lambda0,X0) when we initialize with the exact solution (lambda0,X0)
        //
        X *= (1. / lambda);
      }
    };
  };
}

	

  
//     template <template <class> class ITERATION_CONTROL, 
// 	      template <class, class, class> class ACCELERATION_METHOD=NoInversePowerAcceleration>
//     class Engine{

//     public:
    
//       template <class MATRIX_INVERSIBLE, // X=Y/A exist;
// 		class MATRIX,            // X=A*Y exist;
// 		class VECTOR,
// 		class SOURCE>
//       static inline void apply(const MATRIX_INVERSIBLE & A,  
// 			       const MATRIX & B, 
// 			       typename VECTOR::RealType & lambda, 
// 			       VECTOR & X,
// 			       SOURCE & S,
// 			       const DK::Number & maxExternalIteration,
// 			       const DK::Number & maxNonAcceleratedIteration,
// 			       const DK::Number & maxAcceleratedIteration,
// 			       const typename VECTOR::RealType & fluxExternalPrecision,
// 			       const typename VECTOR::RealType & eigenvaluePrecision,
// 			       const DK::Number & verbose)
//       {
    
// 	typedef typename VECTOR::RealType RealType;
    
// 	RealType dotXX, dotXXold;
    
// 	VECTOR Xold(X);
    
// 	ITERATION_CONTROL< ACCELERATION_METHOD<MATRIX_INVERSIBLE,MATRIX,VECTOR> > iter(A,B,lambda,X,
// 										       maxExternalIteration,
// 										       maxNonAcceleratedIteration,
// 										       maxAcceleratedIteration,
// 										       fluxExternalPrecision,
// 										       eigenvaluePrecision,
// 										       verbose);

// 	dotXXold = dot(X,X);
// 	do {
      
// 	  Xold=X;
      
// 	  S=transpose(B)*X;
      
// 	  X *= lambda;

// 	  X = S/transpose(A); 
      
// 	  dotXX = dot(X,X);
// 	  lambda = sqrt( dotXX / dotXXold );

// 	  X /= lambda;
// 	  dotXXold = dotXX / (lambda * lambda);

// 	} while( !iter.end(X,lambda,Xold,dotXX) ) ;
    
// #ifdef GLASS_COMPUTE_RESIDUAL    

// 	X*=lambda; // X have been scaled
    
// 	// Test . For any non zero vector Y
// 	// lambda=<BtX|Y>/<AtX|Y>
    
// 	VECTOR AX(X);
    
// 	//   1 : build At*X
// 	AX=transpose(A)*X;
    
// 	//   2 : build Bt*X
// 	Xold=transpose(B)*X;
    
// 	//   3 : build a test vector Y
// 	VECTOR Y(X);
// 	Y=1.0;
    
// 	//   4 : check lambda
// 	RealType newLambda=dot(Xold,Y)/dot(AX,Y);
// 	INFOS("newLambda="<<newLambda<<"  newLambda-lambda="<<newLambda-lambda);
    
// #endif    
//       }
    
//     };
//   };

//}

#endif		
