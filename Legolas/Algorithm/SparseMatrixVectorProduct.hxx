/**
 * project  DESCARTES
 *
 * @file     SparseMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPARSEMATRIXVECTORPRODUCT_HXX__
#define __LEGOLAS_SPARSEMATRIXVECTORPRODUCT_HXX__

#include "UTILITES.hxx"
#include "X86Timer.hxx"
#include "Legolas/Matrix/MatrixVectorOperations.hxx"
//#include "Legolas/Vector/Exception.hxx"
#include "stringConvert.hxx"


namespace Legolas{

  // Y=A*X

  class SparseMatrixVectorProduct{
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
      
	// 	if ((A.nrows()==1)&&(A.ncols()==1)){
	// 	  typename MATRIX::FilledRows rows(A);
	// 	  typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(0);
	// 	  if (cols.begin()<cols.end()){
	// 	    ASSIGN_MODE::apply(Y[0],A.sparseGetElement(0,0)*X[0]);
	// 	  }
	// 	}
	// 	else{

	//	INFOS("ICI");

	const int level=MATRIX::level;

	std::string stringBase="SPMV (L=";
	stringBase+=stringConvert(level);
	stringBase+=") ";

	std::string stringInit=stringBase+"initialize";	
	std::string stringGetAIJ=stringBase+"getAIJ";	
	std::string stringAccumulate=stringBase+"accumulate";	


	if (level>-1){

	    
	  //	  INFOS("level="<<level);

	  X86Timer    chronometre;
	  chronometre.start();

	  ASSIGN_MODE::initialize(Y);
	

	  chronometre.stop();
	  chronometre.storeResult(stringInit);
	  //	  chronometre.storeResult("SPmv 0");
	
	  typename MATRIX::FilledRows rows(A);
	  
	  for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	    
	    typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	    
	    for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
	      
	      chronometre.start();
	      typename MATRIX::ConstGetElement aij=A.sparseGetElement(i,j);
	      chronometre.stop();
	      chronometre.storeResult(stringGetAIJ);
	      
	      chronometre.start();
	      //	      INFOS("PROBLEME");
	      ASSIGN_MODE::accumulate(Y[i],aij*X[j]);
	      chronometre.stop();
	      chronometre.storeResult(stringAccumulate);
	      
	    }
	    //	      chronometre.storeResult("SPmv 2");
	    //	    ASSIGN_MODE::accumulate(Y[i],A.sparseGetElement(i,j)*X[j]);
	      
	  }
	}
        else{

	  ASSIGN_MODE::initialize(Y);

	  typename MATRIX::FilledRows rows(A);

	  for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	    
	    typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	    
	    for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
	      
	      ASSIGN_MODE::accumulate(Y[i],A.sparseGetElement(i,j)*X[j]);
	      
	    }
	    
	  }
	}
	  

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
	    //	    TransposeAnyMatrixVectorMultExpression expr=transpose(A.sparseGetElement(0,0))*X[0];

	    //	    Y[0]=expr;
	    //	    Y[0]=transpose(A.sparseGetElement(0,0))*X[0];

	    //	    INFOS("PROBLEME");
	    ASSIGN_MODE::apply(Y[0],transpose(A.sparseGetElement(0,0))*X[0]);
	  }
	  else{

	    ASSIGN_MODE::initialize(Y);
	  
	    typename MATRIX::FilledRows rows(A);
	  
	    for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	  
	      typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	    
	      for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
		//	INFOS("PROBLEME");
		//		INFOS("level :"<<A.getLevel()<<" i="<<i<<" j="<<j<<" Y[j]="<<Legolas::squareNorm2(Y[j])<<" (X[i]="<<Legolas::squareNorm2(X[i])<<")");
		ASSIGN_MODE::accumulate(Y[j],transpose(A.sparseGetElement(i,j))*X[i]);
		//		INFOS("level :"<<A.getLevel()<<" i="<<i<<" j="<<j<<" Y[j]="<<Legolas::squareNorm2(Y[j])<<" (X[i]="<<Legolas::squareNorm2(X[i])<<")");



	      
	      }
	    }
	  
	  }

	  return ;
	}
      
      };
    };
  };
}

#endif


  //   template <class MATRIX,class VECTOR, class VECTOR_INOUT>
  //   static inline void apply(const MATRIX & A , 
  // 			   const VECTOR & X ,
  // 			   VECTOR_INOUT & Y)
  //   {

  //     //    INFOS("on est la");
  //     if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }

  //     X86_Timer chronos;

  //     if ((A.nrows()==1)&&(A.ncols()==1)){
  //       //INFOS("ICI");
  //       Y[0]=A(0,0)*X[0];
  //     }
  //     else{

  //       chronos.start();

  //       typename VECTOR_INOUT::Element s(Y[0]);

  //       chronos.stop();
  //       INFOS("Total duration :"<<chronos.get_elapsed_time_in_second());
      

  //       typename MATRIX::FilledRows rows(A);
    
  //       for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
      
  // 	s=0.0;
  // 	typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	
  // 	for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
	  
  // 	  s+=A.sparseGetElement(i,j)*X[j];
	  
  // 	}

  // 	Y[i]=s;

  //       }
  //     }
    
  //     return ;
  //   }

  //   template<class ASSIGN_MODE>
  //   class Engine{
  //   public:


  //     template <class MATRIX,class VECTOR, class VECTOR_INOUT>
  //     static inline void apply(const MATRIX & A , 
  // 			     const VECTOR & X ,
  // 			     VECTOR_INOUT & Y)
  //     {
      
  //       //      INFOS("A.nrows()="<<A.nrows());

  //       X86_Timer chronos;

  //       if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
      
  //       if ((A.nrows()==1)&&(A.ncols()==1)){
  // 	//	INFOS("ICI");
  // 	ASSIGN_MODE::apply(Y[0],A(0,0)*X[0]);
  //       }
  //       else{

  // 	chronos.start();

  // 	typename VECTOR_INOUT::Element s(Y[0]);

  // 	chronos.stop();

  // 	chronos.storeResult("Copy de s");
	
  // 	typename MATRIX::FilledRows rows(A);
	
  // 	for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	  
  // 	  chronos.start();

  // 	  s=0.0;

  // 	  chronos.stop();

  // 	  chronos.storeResult("s=0.0");

  // 	  typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	  
  // 	  for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
	    
  // 	    s+=A.sparseGetElement(i,j)*X[j];
	    
  // 	  }

  // 	  chronos.start();	  

  // 	  ASSIGN_MODE::apply(Y[i],s);

  // 	  chronos.stop();

  // 	  chronos.storeResult("Y[i]=s");
	  
  // 	}
  //       }
    
  //       return ;
  //     }

  //     // Warning the following code is not correct and does only apply to EqualAssign Mode !!!
  //     template <class MATRIX,class VECTOR, class VECTOR_INOUT>
  //     static inline void applyBis(const MATRIX & A , 
  // 			     const VECTOR & X ,
  // 			     VECTOR_INOUT & Y)
  //     {
      
  //       //      INFOS("on est la");
  //       if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
      
  //       if ((A.nrows()==1)&&(A.ncols()==1)){
  // 	//INFOS("ICI");
  // 	ASSIGN_MODE::apply(Y[0],A(0,0)*X[0]);
  //       }
  //       else{
	
  // 	typename MATRIX::FilledRows rows(A);
	
  // 	for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	  
  // 	  // Warning the following code is not correct and does only apply to EqualAssign Mode !!!
  // 	  typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);

  // 	  int j=cols.begin();

  // 	  Y[i]=A.sparseGetElement(i,j)*X[j];

  // 	  cols.next(j);

  // 	  for ( ; j< cols.end() ; cols.next(j)){
	    
  // 	    Y[i]+=A.sparseGetElement(i,j)*X[j];
	    
  // 	  }
	  
  // 	}
  //       }
    
  //       return ;
  //     }

  //   };


