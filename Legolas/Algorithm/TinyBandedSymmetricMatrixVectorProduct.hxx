/**
 * project  DESCARTES
 *
 * @file     TinyBandedSymmetricMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TINYBANDEDSYMMETRICMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_TINYBANDEDSYMMETRICMATRIXVECTORPRODUCT_HXX__

#include "UTILITES.hxx"
#include "X86Timer.hxx"

namespace Legolas{

  // Y=A*X

  class TinyBandedSymmetricMatrixVectorProduct{
  public:

    template<class ASSIGN_MODE>
    class Engine{
    public:



      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void applySlow(const MATRIX & A , 
				   const VECTOR & X ,
				   VECTOR_INOUT & Y)
      {
	//	X86Timer    chronometre;

	//	chronometre.start();
	ASSIGN_MODE::initialize(Y);
	//	chronometre.stop();
	//	chronometre.storeResult("TinyBandedSymmetricMatrixVectorProduct initialize");

	const int nrows=A.nrows();
	const int hbw=A.getHalfBandwidth();
	
	for (int i=0; i< nrows ; i++){
	    
	  const int jmin=std::max(0,i-hbw);

	  for (int j=jmin; j< i ; j++){
	    const typename VECTOR::RealType aij=A.lowerBandedGetElement(i,j);

	    ASSIGN_MODE::accumulate(Y[i],aij*X[j]);
	    ASSIGN_MODE::accumulate(Y[j],aij*X[i]);
	  }

	  ASSIGN_MODE::accumulate(Y[i],A.lowerBandedGetElement(i,i)*X[i]);

	  //	  const int jmax=std::min(i+hbw+1,nrows);
	  
	  //	  for (int j=i; j<jmax ; j++){

	  //	  }
	}
      }


      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void applyUpperBorder(const int nrows,
					  const int hbw,
					  const MATRIX & A, 
					  const VECTOR & X ,
					  VECTOR_INOUT & Y){
	for (int i=0; i< std::min(nrows,hbw) ; i++){
	  Y[i]*=ASSIGN_MODE::beta();
	  const int jmin=std::max(0,i-hbw);

	  for (int j=jmin; j< i ; j++){
	    const typename VECTOR::RealType aij=A.lowerBandedGetElement(i,j);
	    ASSIGN_MODE::accumulate(Y[i],aij*X[j]);
	    ASSIGN_MODE::accumulate(Y[j],aij*X[i]);
	  }
	  ASSIGN_MODE::accumulate(Y[i],A.lowerBandedGetElement(i,i)*X[i]);
	}
      }
	
      
      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void applyTriDiag(const MATRIX & A , 
				      const VECTOR & X ,
				      VECTOR_INOUT & Y)
      {
	const int nrows=A.nrows();

	applyUpperBorder(nrows,1,A,X,Y);
	
	for (int i=1; i< nrows ; i++){
	  
	  const typename VECTOR::RealType aij=A.lowerBandedGetElement(i,i-1);
	  const typename VECTOR::RealType aii=A.lowerBandedGetElement(i,i);
	  
	  Y[i]*=ASSIGN_MODE::beta();
	  
	  ASSIGN_MODE::accumulate(Y[i-1],aij*X[i]);
	  ASSIGN_MODE::accumulate(Y[i],aij*X[i-1]);
	  ASSIGN_MODE::accumulate(Y[i],aii*X[i]);
	}
      }

      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void applyPentaDiag(const MATRIX & A , 
					const VECTOR & X ,
					VECTOR_INOUT & Y)
      {

	//	X86Timer    chronometre;
	//	chronometre.start();

	const int nrows=A.nrows();	

	applyUpperBorder(nrows,2,A,X,Y);
	
	for (int i=2; i< nrows ; i++){

	  const typename VECTOR::RealType aiim2=A.lowerBandedGetElement(i,i-2);
	  const typename VECTOR::RealType aiim1=A.lowerBandedGetElement(i,i-1);
	  const typename VECTOR::RealType aii=A.lowerBandedGetElement(i,i);

// 	  ASSIGN_MODE::accumulate(Y[i-2],aiim2*X[i]);
// 	  ASSIGN_MODE::accumulate(Y[i-1],aiim1*X[i]);

// 	  Y[i]*=ASSIGN_MODE::beta();

// 	  ASSIGN_MODE::accumulate(Y[i],aiim2*X[i-2]);
// 	  ASSIGN_MODE::accumulate(Y[i],aiim1*X[i-1]);
// 	  ASSIGN_MODE::accumulate(Y[i],aii*X[i]);

	  Y[i-2]+=aiim2*X[i];
	  Y[i-1]+=aiim1*X[i];


	  Y[i]=aiim2*X[i-2]+aiim1*X[i-1]+aii*X[i];



	}
	//	chronometre.stop();
	//	chronometre.storeResult("Pentadiag");
      }

      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void applyHeptaDiag(const MATRIX & A , 
					const VECTOR & X ,
					VECTOR_INOUT & Y)
      {
	const int nrows=A.nrows();
	
	applyUpperBorder(nrows,3,A,X,Y);
	
	for (int i=3; i< nrows ; i++){

	  const typename VECTOR::RealType aiim3=A.lowerBandedGetElement(i,i-3);
	  const typename VECTOR::RealType aiim2=A.lowerBandedGetElement(i,i-2);
	  const typename VECTOR::RealType aiim1=A.lowerBandedGetElement(i,i-1);
	  const typename VECTOR::RealType aii=A.lowerBandedGetElement(i,i);

	  ASSIGN_MODE::accumulate(Y[i-3],aiim3*X[i]);
	  ASSIGN_MODE::accumulate(Y[i-2],aiim2*X[i]);
	  ASSIGN_MODE::accumulate(Y[i-1],aiim1*X[i]);

	  Y[i]*=ASSIGN_MODE::beta();

	  ASSIGN_MODE::accumulate(Y[i],aiim3*X[i-3]);
	  ASSIGN_MODE::accumulate(Y[i],aiim2*X[i-2]);
	  ASSIGN_MODE::accumulate(Y[i],aiim1*X[i-1]);

	  ASSIGN_MODE::accumulate(Y[i],aii*X[i]);

	}
      }

      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const MATRIX & A , 
			       const VECTOR & X ,
			       VECTOR_INOUT & Y)
      {

	const int hbw=A.getHalfBandwidth();
	
	switch(hbw){
	  
	case 1://TriDiag
	  applyTriDiag(A,X,Y);
	  break;
	case 2://PentaDiag
	  applyPentaDiag(A,X,Y);
	  break;
	case 3://HeptaDiag
	  //	  applyHeptaDiag(A,X,Y);
	  applySlow(A,X,Y);
	  break;
	default:
	  applySlow(A,X,Y);
	}
	
      }

      


    };
  };
}

#endif
