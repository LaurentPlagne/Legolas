/**
 * project  DESCARTES
 *
 * @file     QuadMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_QUADMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_QUADMATRIXVECTORPRODUCT_HXX__

#include "X86Timer.hxx"

namespace Legolas{

  //A=D+R => AX=DX+RX

  class QuadMatrixVectorProduct{
  public :

    template <class ASSIGN_MODE>
    class Engine{
    public:
      template <class QUAD_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const QUAD_MATRIX & A, 
			       const VECTOR & X,
			       VECTOR_INOUT & Y)
      {

	X86Timer chronos;
	//      INFOS("Passage dans QuadMatrixVectorProduct") ;
	if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }

	chronos.start();
	ASSIGN_MODE::initialize(Y);
	chronos.stop(); chronos.storeResult("ASSIGN_MODE::initialize(Y)");
	chronos.start();
	ASSIGN_MODE::accumulate(Y[0],A.M00()*X[0]);
	chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[0],A.M00()*X[0])");
	chronos.start();
	ASSIGN_MODE::accumulate(Y[0],A.M01()*X[1]);
	chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[0],A.M01()*X[1])");
	chronos.start();
	ASSIGN_MODE::accumulate(Y[1],A.M10()*X[0]);
	chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[1],A.M10()*X[0])");
	chronos.start();
	ASSIGN_MODE::accumulate(Y[1],A.M11()*X[1]);
	chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[1],A.M11()*X[1])");
	



	return ;
      }
    };

    // 

    class Transpose{
    public:
      template <class ASSIGN_MODE>
      class Engine{
      public:
	template <class QUAD_MATRIX,class VECTOR, class VECTOR_INOUT > 
	static inline void apply(const QUAD_MATRIX & A, 
				 const VECTOR & X,
				 VECTOR_INOUT & Y)
	{
	  if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
	  X86Timer chronos;

	  chronos.start();
	  ASSIGN_MODE::initialize(Y);
	  chronos.stop(); chronos.storeResult("ASSIGN_MODE::initialize(Y);");
	  chronos.start();
	  ASSIGN_MODE::accumulate(Y[0],transpose(A.M00())*X[0]);
	  chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[0],transpose(A.M00())*X[0]);");
	  chronos.start();
	  ASSIGN_MODE::accumulate(Y[0],transpose(A.M10())*X[1]);
	  chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[0],transpose(A.M10())*X[1]);");
	  chronos.start();
	  ASSIGN_MODE::accumulate(Y[1],transpose(A.M01())*X[0]);
	  chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[1],transpose(A.M01())*X[0]);");
	  chronos.start();
	  ASSIGN_MODE::accumulate(Y[1],transpose(A.M11())*X[1]);
	  chronos.stop(); chronos.storeResult("ASSIGN_MODE::accumulate(Y[1],transpose(A.M11())*X[1]);");


	  return;
	}
      };
    };
  };
}

#endif	
