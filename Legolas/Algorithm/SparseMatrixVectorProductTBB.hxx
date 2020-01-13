/**
 * project  DESCARTES
 *
 * @file     SparseMatrixVectorProductTBB.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPARSEMATRIXVECTORPRODUCTTBB_HXX__
#define __LEGOLAS_SPARSEMATRIXVECTORPRODUCTTBB_HXX__

#include "UTILITES.hxx"
#include "X86Timer.hxx"

#include "my_tbb_parallel_for.hxx"

//#include "tbb/task_scheduler_init.h"
//#include "tbb/parallel_for.h"
//#include "tbb/blocked_range.h"



namespace Legolas{

  // Y=A*X
  template <class ASSIGN_MODE, class MATRIX,class VECTOR, class VECTOR_INOUT>
  class LineFunctor{
  private:
    const MATRIX & A_;
    const VECTOR & X_;
    VECTOR_INOUT & Y_;
  public:
    LineFunctor(const MATRIX & A , 
		const VECTOR & X ,
		VECTOR_INOUT & Y):A_(A),X_(X),Y_(Y){
    }
    
    
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      
      typename MATRIX::FilledRows rows(A_);
      
      for (int i=r.begin() ; i!=r.end() ; i++){
	
	ASSIGN_MODE::initialize(Y_[i]);
	
	typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	
	for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
	  
	  ASSIGN_MODE::accumulate(Y_[i],A_.sparseGetElement(i,j)*X_[j]);
	  
	}
	
      }
    }
  };
  




  class SparseMatrixVectorProductTBB{
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

	//	my_tbb::task_scheduler_init init(SPN_THREAD_NUMBER);
	//	my_tbb::task_scheduler_init init(spn_thread_number()); 

	LineFunctor<ASSIGN_MODE,MATRIX,VECTOR,VECTOR_INOUT> lf(A,X,Y);

	//	my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows(),100),lf);
	my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),lf);
	  

      }

	 
      }; 



	
// 	ASSIGN_MODE::initialize(Y);

// 	typename MATRIX::FilledRows rows(A);

// 	for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	  
// 	  typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	  
// 	  for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
	    
// 	    ASSIGN_MODE::accumulate(Y[i],A.sparseGetElement(i,j)*X[j]);
	    
// 	  }
	  
// 	}

	
//      }
      
//    };

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
	    //	INFOS("ICI");
	    ASSIGN_MODE::apply(Y[0],transpose(A.sparseGetElement(0,0))*X[0]);
	  }
	  else{

	    ASSIGN_MODE::initialize(Y);
	  
	    typename MATRIX::FilledRows rows(A);
	  
	    for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	  
	      typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	    
	      for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
	      
		ASSIGN_MODE::accumulate(Y[j],transpose(A.sparseGetElement(i,j))*X[i]);
	      
	      }
	    }
	  
	  }

	  return ;
	}
      
      };
    };
  };



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





}

#endif
