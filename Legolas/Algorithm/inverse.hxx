#ifndef __LEGOLAS_INVERSE_HXX__
#define __LEGOLAS_INVERSE_HXX__

// Iterative inverse Algorithm : This algorithm solves the Problem Ax = BX + S

#include "Legolas/Algorithm/NoIterativeLinearSystemAcceleration.hxx"
#include "X86Timer.hxx"

namespace Legolas{

  class inverse
  {
  public:
    template <template <class> class ITERATION_CONTROL, template <class, class, class> class ACCELERATION_METHOD=NoIterativeLinearSystemAcceleration>
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
			       const typename VECTOR::RealType & fluxExternalPrecision,
			       const DK::Number & verbose
			       )
      {
	typedef typename VECTOR::RealType    RealType;
	VECTOR                      Xold(X);
	VECTOR                      Xinter(S);

	typedef ACCELERATION_METHOD< MATRIX_INVERSIBLE,MATRIX,VECTOR >    Acceleration;
	typedef ITERATION_CONTROL< Acceleration >                         IterationControl;

	IterationControl    iter(A,B,lambda,X,
				 maxExternalIteration,
				 fluxExternalPrecision,
				 verbose);

	X86Timer    chronometre;
	bool finished = false;

        do
	  {
	    chronometre.start();
	    Xinter += B * X; 
	    chronometre.stop();
	    chronometre.storeResult("Inverse Xinter =B*X+S");

            chronometre.start();
	    X = Xinter / A;
            chronometre.stop();
	    chronometre.storeResult("Inverse X = Xinter / A");

	    chronometre.start();
	    finished=iter.end(X,Xold); 
	    chronometre.stop();
	    chronometre.storeResult("Inverse iter.end(X,Xold)");

            chronometre.start();
	    Xold=X;
	    chronometre.stop();
	    chronometre.storeResult("Inverse Xold=X");
	    
	    chronometre.start();
	    Xinter = S;
	    chronometre.stop();
	    chronometre.storeResult("Inverse Xinter=S");
						
	  } while( !finished ) ;
	
#ifdef GLASS_COMPUTE_RESIDUAL
	//
	VECTOR    AX(X);
	//
	AX = A * X;
	AX -= B * X;
	AX -= S;
	RealType    residu = dot(AX,AX);
	INFOS( "residu = " << residu << std::endl );
#endif
      }
    };
    
  };
  
}

#endif



