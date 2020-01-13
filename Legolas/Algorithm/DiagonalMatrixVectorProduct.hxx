/**
 * project  DESCARTES
 *
 * @file     DiagonalMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DIAGONALMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_DIAGONALMATRIXVECTORPRODUCT_HXX__

#include "UTILITES.hxx"
#include "X86Timer.hxx"

//#define TIME_DMVP

namespace Legolas{

  // Y=A*X

  class DiagonalMatrixVectorProduct{
  public:

    template<class ASSIGN_MODE>
    class Engine{
    public:


      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const MATRIX & A , 
			       const VECTOR & X ,
			       VECTOR_INOUT & Y)
      {
      
	//	INFOS("ENTERING DiagonalMatrixVectorProduct");

	if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
      
	if ((A.nrows()==1)&&(A.ncols()==1)){
	  //	  INFOS("level :"<<A.getLevel()<<" Y[0]="<<Legolas::squareNorm2(Y[0])<<" (X[0]="<<Legolas::squareNorm2(X[0])<<")");
	  ASSIGN_MODE::apply(Y[0],A.diagonalGetElement(0)*X[0]);
	  //	  INFOS("level :"<<A.getLevel()<<" Y[0]="<<Legolas::squareNorm2(Y[0])<<" (X[0]="<<Legolas::squareNorm2(X[0])<<")");
	}
	else{

	  
#ifdef TIME_DMVP
	  X86Timer chronos;
#endif


	  //	  const int n=A.nrows();
	  
	  const int nrows=A.nrows();

	  for (int i=0 ; i < nrows ; ++i){

#ifdef TWO_PARTS

	  typedef typename MATRIX::ConstGetElement::MatrixVectorProduct MVP;
	  typedef typename MVP::template Engine< EqualAssign<double &> > MVPE;


	    chronos.start();
	    
	    const typename MATRIX::ConstGetElement & aii=A.diagonalGetElement(i);

	    chronos.stop();
	    chronos.storeResult("DiagonalMVP part 1");
	    

	    chronos.start();

	    //	    ASSIGN_MODE::apply(Y[i],aii*X[i]);

	    MVPE::apply(aii,X[i],Y[i]);

	    //	    ASSIGN_MODE::apply(Y[i],aii*X[i]);

	    //	    ASSIGN_MODE::apply(Y[i],A.diagonalGetElement(i)*X[i]);


	    chronos.stop();
	    chronos.storeResult("DiagonalMVP part 2");

#else
#ifdef TIME_DMVP
	    chronos.start();
#endif
	    //	    INFOS("level :"<<A.getLevel()<<" i="<<i<<" Y[i]="<<Legolas::squareNorm2(Y[0])<<" (X[i]="<<Legolas::squareNorm2(X[i])<<")");
	    ASSIGN_MODE::apply(Y[i],A.diagonalGetElement(i)*X[i]);
	    //	    INFOS("level :"<<A.getLevel()<<" i="<<i<<" Y[i]="<<Legolas::squareNorm2(Y[0])<<" (X[i]="<<Legolas::squareNorm2(X[i])<<")");
	    //	    MVPE::apply(A.diagonalGetElement(i),X[i],Y[i]);

#ifdef TIME_DMVP
	    chronos.stop();
	    chronos.storeResult("DiagonalMVP part 1+2");
#endif

#endif

	  }



	}
    
	return ;
      }

    };

    class Transpose{
    public:
      template<class ASSIGN_MODE>
      class Engine{
      public:
	template <class MATRIX,class VECTOR, class VECTOR_INOUT>
	static inline void apply(const MATRIX & A , 
				 const VECTOR & X ,
				 VECTOR_INOUT & Y)
	{

	  if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
	
	  if ((A.nrows()==1)&&(A.ncols()==1)){
	    //	    INFOS("level :"<<A.getLevel()<<" Y[0]="<<Legolas::squareNorm2(Y[0])<<" (X[0]="<<Legolas::squareNorm2(X[0])<<")");
	    ASSIGN_MODE::apply(Y[0],transpose(A.diagonalGetElement(0))*X[0]);
	    //	    INFOS("level :"<<A.getLevel()<<" Y[0]="<<Legolas::squareNorm2(Y[0])<<" (X[0]="<<Legolas::squareNorm2(X[0])<<")");
	  }
	  else{
	  
	    //	    ASSIGN_MODE::initialize(Y);
	    for (int i=0 ; i < A.nrows() ; i++){
	      //	      INFOS("level :"<<A.getLevel()<<" i="<<i<<" Y[i]="<<Legolas::squareNorm2(Y[i])<<" (X[i]="<<Legolas::squareNorm2(X[i])<<")");
	      ASSIGN_MODE::apply(Y[i],transpose(A.diagonalGetElement(i))*X[i]);
	      //	      INFOS("level :"<<A.getLevel()<<" i="<<i<<" Y[i]="<<Legolas::squareNorm2(Y[i])<<" (X[i]="<<Legolas::squareNorm2(X[i])<<")");
	    
	    }
	  }
	  
	  return ;

	}      
      };
    };
  };


  // class DiagonalMatrixVectorProductBis{
  // public:

  //   template<class ASSIGN_MODE>
  //   class Engine{
  //   public:


  //     template <class MATRIX,class VECTOR, class VECTOR_INOUT>
  //     static inline void apply(const MATRIX & A , 
  // 			       const VECTOR & X ,
  // 			       VECTOR_INOUT & Y)
  //     {
  // 	for (int i=0 ; i < A.nrows() ; ++i){
  // 	  ASSIGN_MODE::apply(Y[i],A.diagonalGetElement(i)*X[i]);
  // 	}	
	
  // 	return ;
  //     }

  //   };

  //   class Transpose{
  //   public:
  //     template<class ASSIGN_MODE>
  //     class Engine{
  //     public:
  // 	template <class MATRIX,class VECTOR, class VECTOR_INOUT>
  // 	static inline void apply(const MATRIX & A , 
  // 				 const VECTOR & X ,
  // 				 VECTOR_INOUT & Y)
  // 	{
  // 	  INFOS("ICI");
  // 	  throw std::runtime_error("ICI");
  // 	  return ;
	  
  // 	}      
  //     };
  //   };
  // };

  // class DiagonalMatrixVectorProductTer{
  // public:

  //   template<class ASSIGN_MODE>
  //   class Engine{
  //   public:


  //     template <class MATRIX,class VECTOR, class VECTOR_INOUT>
  //     static inline void apply(const MATRIX & A , 
  // 			       const VECTOR & X ,
  // 			       VECTOR_INOUT & Y)
  //     {
  // 	for (int i=0 ; i < A.nrows() ; ++i){
  // 	  ASSIGN_MODE::apply(Y[i],A.diagonalGetElement(i)*X[i]);
  // 	}
  // 	return ;
  //     }

  //   };

  //   class Transpose{
  //   public:
  //     template<class ASSIGN_MODE>
  //     class Engine{
  //     public:
  // 	template <class MATRIX,class VECTOR, class VECTOR_INOUT>
  // 	static inline void apply(const MATRIX & A , 
  // 				 const VECTOR & X ,
  // 				 VECTOR_INOUT & Y)
  // 	{
  // 	  INFOS("ICI");
  // 	  throw std::runtime_error("");
  // 	  return ;
	  
  // 	}      
  //     };
  //   };
  // };

}

#endif
