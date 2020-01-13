/**
 * project  DESCARTES
 *
 * @file     DRConjugateGradient.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRCONJUGATEGRADIENT_HXX__
#define __LEGOLAS_DRCONJUGATEGRADIENT_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"
#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "NonLinearSystemAcceleration.hxx"
//#include "Legolas/Algorithm/AssignCopyTraits.hxx"

// Solves AX=B 
// with A=D+R => AX=DX+RX
// A diagonal preconditionner is used
//  One solves (D^-1)AX=(D^-1)B
//  (D^-1)AX=(D^-1)(D+R)X=X+RX/D

class DRConjugateGradient{
public :

  template <class ASSIGN_MODE>
  class Engine{
  public:

    template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT>
    static inline void apply(const DR_MATRIX & A, 
			     const VECTOR & B,
			     VECTOR_INOUT & X)
    {

      typedef typename DR_MATRIX::DMatrixInterface DMatrixInterface;
      typedef typename DR_MATRIX::RMatrixInterface RMatrixInterface;
      typename DMatrixInterface::ConstView D(A.D());
      typename RMatrixInterface::ConstView R(A.R());

      typedef typename DR_MATRIX::RealType RealType;

      typedef NonLinearSystemAcceleration<DR_MATRIX,VECTOR,VECTOR_INOUT> Acceleration;
      typedef DefaultLinearSystemIterationControl<Acceleration> IterationControl;

      IterationControl iter(A,B,X);

      typename VECTOR::Copy G(B);

      G-=A*X;
      
      typename VECTOR::Copy Z(B);

      Z=G/D;
      
      typename VECTOR::Copy H(Z);

      typename VECTOR::Copy AH(B);

      RealType dotZG=VECTOR::dot(Z,G);

      do {

	AH=A*H;

	RealType alpha=dotZG/VECTOR::dot(H,AH);

	X+=alpha*H;

	alpha*=-1.0;

	G+=alpha*AH;

	Z=G/D;

	RealType newDotZG=VECTOR::dot(Z,G);
	RealType beta=newDotZG/dotZG;

	H*=beta;
	H+=Z;

	dotZG=newDotZG;

     }while(!iter.end(X));
      
    }
    
  };

};




endif endif
