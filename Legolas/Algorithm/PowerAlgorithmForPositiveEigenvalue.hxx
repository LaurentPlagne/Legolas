#ifndef __LEGOLAS_POWERALGORITHMFORPOSITIVEEIGENVALUE_HXX__
#define __LEGOLAS_POWERALGORITHMFORPOSITIVEEIGENVALUE_HXX__

// New Power Algorithm : This algorithm solves the Generalized Eigenvalue Problem
//                   find the biggest in modulus lambda such as :  Ax=(1/lambda)Bx when this value is positive.

#include "Legolas/Algorithm/NoInversePowerAcceleration.hxx"
//#include "NoInversePowerAcceleration.hxx"
#include "X86Timer.hxx"


namespace Legolas{

  class PowerAlgorithmForPositiveEigenvalue
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
			       int & lastIterationNumber
			       )
      {
	typedef typename VECTOR::RealType    RealType;
	RealType                             dotSS, dotSSold;
	VECTOR                      Sold(S);

	typedef ACCELERATION_METHOD< MATRIX_INVERSIBLE,MATRIX,VECTOR >    Acceleration;
	typedef ITERATION_CONTROL< Acceleration >                         IterationControl;

	//	IterationControl    iter(A,B,lambda,X,
	// modif LP X->S
	IterationControl    iter(A,B,lambda,S,
				 maxExternalIteration,
				 maxNonAcceleratedIteration,
				 maxAcceleratedIteration,
				 fluxExternalPrecision,
				 eigenvaluePrecision,
				 verbose);



	X86Timer    chronometre;
	bool finished=false;

	S = B * X;
	X*=lambda; // The current have to be multiplied by the eigenvalue in order to have a good initialisation of the iterative Gauss-Seildel process
	dotSSold = Legolas::dot(S,S);

	//INFOS("A.getStructureName()="<<A.getStructureName());
	//INFOS("A(0,0).getStructureName()="<<A.sparseGetElement(0,0).getStructureName());


	typename MATRIX_INVERSIBLE::Solver linearSolver(A,X,S);

	do
	  {

	    chronometre.start();
	    //	    X = S / A;
	    linearSolver.solve(A,X,S);
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo X =S/A");

	    chronometre.start();
	    //	    Sold=S;
	    Legolas::swap(Sold,S);
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo Sold=S");

	    chronometre.start();
	    S = B * X; // = lambda*Sn+1
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo S =B*X");

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
	    finished=iter.end(S,lambda,Sold,dotSSold); // P.G. new : dotSSold au lieu de dotSS
	    chronometre.stop();
	    chronometre.storeResult("PowerAlgo iter.end(S,lambda,Sold,dotSS)");
						
	    
	  } while( !finished ) ;

        lastIterationNumber=iter.getCurrentIteration();

	//
        // it is done in order to leave the program with (lambda0,X0) when we initialize with the exact solution (lambda0,X0)
        //
        X *= (1./lambda);
          	
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
	
	VECTOR TOTO(X);
	TOTO = AX / A;
        //
	//   2 : build B*X
	//
	Sold = B * X;
        //
	//   3 : build a test vector Y
	//
	VECTOR    Y(X);
	Y = 1.0;
	//
	//   4 : check lambda
	//
	RealType    newLambda = Legolas::dot(Sold,Y) / Legolas::dot(AX,Y);
	INFOS( "newLambda=" << newLambda << "  newLambda - lambda = "<< newLambda - lambda );
#endif
      }
    };
    
  };
  
}

#endif



