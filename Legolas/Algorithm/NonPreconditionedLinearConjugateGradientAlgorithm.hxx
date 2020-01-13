/**
 * project  DESCARTES
 *
 * @file     NonPreconditionedLinearConjugateGradientAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_NONPRECONDITIONEDLINEARCONJUGATEGRADIENTALGORITHM_HXX__
#define __LEGOLAS_NONPRECONDITIONEDLINEARCONJUGATEGRADIENTALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"
#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"

namespace Legolas{

  class NonPreconditionedLinearConjugateGradientAlgorithm{
  public :

    template <class ASSIGN_MODE>
    class Engine{
    public:

      template <class SPARSE_MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const SPARSE_MATRIX & A, 
			       const VECTOR & B,
			       VECTOR_INOUT & X)
      {

	typedef typename SPARSE_MATRIX::RealType RealType;
	typedef NoLinearSystemAcceleration<SPARSE_MATRIX,VECTOR,VECTOR_INOUT> Acceleration;
	typedef DefaultLinearSystemIterationControl<Acceleration> IterationControl;
	IterationControl iter(A,B,X);

	VECTOR G(B);

	G=A*X;
	G-=B;

	VECTOR H(G);
	VECTOR AH(G);

	RealType dGG=dot(G,G);

	do {

	  AH=A*H;

	  RealType c=-1.0*dot(H,G)/dot(H,AH);

	  X+=c*H;

	  G+=c*AH;

	  RealType newdGG=dot(G,G);
	  H*=newdGG/dGG;
	  H+=G;

	  dGG=newdGG;

	}while(!iter.end(X));
      
	INFOS("Linear Conjugate Gradient Algoritm has converged within "<<iter.getCurrentIteration()<<" iterations");
      
      }
    
    };

  };




}

#endif
