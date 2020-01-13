/**
 * project  DESCARTES
 *
 * @file     DiagonalMatrixVectorProductTBB.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DIAGONALMATRIXVECTORPRODUCTTBB_HXX__
#define __LEGOLAS_DIAGONALMATRIXVECTORPRODUCTTBB_HXX__

#include "UTILITES.hxx"
#include "X86Timer.hxx"

#include "my_tbb_parallel_for.hxx"
//#include "tbb/tbb_stddef.h"
//extern "C" int TBB_runtime_interface_version();

namespace Legolas{



  // Y=A*X

  class DiagonalMatrixVectorProductTBB{
  public:

    template<class ASSIGN_MODE>
    class Engine{
    public:
 

      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      class MultFunctor{
      private:
	const MATRIX & A_;
	const VECTOR & X_;
        VECTOR_INOUT & Y_;
      public:
	MultFunctor(const MATRIX & A , 
		    const VECTOR & X ,
		    VECTOR_INOUT & Y):A_(A),X_(X),Y_(Y){}
	MultFunctor(const MultFunctor & source):A_(source.A_),X_(source.X_),Y_(source.Y_){}
	
	
	inline void operator()(const my_tbb::blocked_range<int> & r) const {
	  for (int i=r.begin() ; i!=r.end() ; i++){
	    ASSIGN_MODE::apply(Y_[i],A_.diagonalGetElement(i)*X_[i]);
	  }
	}
      };
      


      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const MATRIX & A , 
			       const VECTOR & X ,
			       VECTOR_INOUT & Y)
      {
      
	
	//	INFOS("ICI");
	//	INFOS("A.nrows()="<<A.nrows());
	//	throw std::runtime_error("");

	//	my_tbb::task_scheduler_init init(spn_thread_number()); 
	//	INFOS("tbb_version="<<TBB_runtime_interface_version());

	//	INFOS("ENTERING DiagonalMatrixVectorProduct");

	if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
      
	if ((A.nrows()==1)&&(A.ncols()==1)){
	  //	  INFOS("level :"<<A.getLevel()<<" Y[0]="<<Legolas::squareNorm2(Y[0])<<" (X[0]="<<Legolas::squareNorm2(X[0])<<")");
	  ASSIGN_MODE::apply(Y[0],A.diagonalGetElement(0)*X[0]);
	  //	  INFOS("level :"<<A.getLevel()<<" Y[0]="<<Legolas::squareNorm2(Y[0])<<" (X[0]="<<Legolas::squareNorm2(X[0])<<")");
	}
	else{

	  //	  INFOS("A.nrows()="<<A.nrows());

	  //	  my_tbb::task_scheduler_init init(SPN_THREAD_NUMBER);
	  //	  my_tbb::task_scheduler_init init(spn_thread_number()); 


	  typedef MultFunctor<MATRIX,VECTOR,VECTOR_INOUT> MF;

	  X86Timer chronos;
	  chronos.start();

	  //	  my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows(),100),MF(A,X,Y));


	  my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),MF(A,X,Y));
	  
	  chronos.stop();
	  chronos.storeResult("my_tbb::parallel_for");
	    
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



  class DiagonalMatrixVectorProductTBBBis{
  public:

    template<class ASSIGN_MODE>
    class Engine{
    public:
 

      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      class MultFunctor{
      private:
	//LP Do not change this
	const MATRIX & A_;
	//	const MATRIX & A_;
	const VECTOR & X_;
        VECTOR_INOUT & Y_;
      public:
	MultFunctor(const MATRIX & A , 
		    const VECTOR & X ,
		    VECTOR_INOUT & Y):A_(A),X_(X),Y_(Y){}


	MultFunctor(const MultFunctor & source):A_(source.A_),X_(source.X_),Y_(source.Y_){
	  //	  INFOS("ON EST LA");
	}
	
	
	inline void operator()(const my_tbb::blocked_range<int> & r) const {
	  for (int i=r.begin() ; i!=r.end() ; i++){
	    ASSIGN_MODE::apply(Y_[i],A_.diagonalGetElement(i)*X_[i]);
	  }
	}
      };
      


      template <class MATRIX,class VECTOR, class VECTOR_INOUT>
      static inline void apply(const MATRIX & A , 
			       const VECTOR & X ,
			       VECTOR_INOUT & Y)
      {
      

	if ( &(Y.getRef())==&(X.getRef()) ){ INFOS("Aliasing Problem !!!") ; throw std::runtime_error("Aliasing Problem !!!"); }
      
	if ((A.nrows()==1)&&(A.ncols()==1)){
	  ASSIGN_MODE::apply(Y[0],A.diagonalGetElement(0)*X[0]);
	}
	else{

	  //	  my_tbb::task_scheduler_init init(spn_thread_number()); 

	  //	  VECTOR_INOUT Ybis(Y);
	  
	  

	  typedef MultFunctor<MATRIX,VECTOR,VECTOR_INOUT> MF;

	  X86Timer chronos;
	  chronos.start();

	  //	  my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows(),A.nrows()),MF(A,X,Y));
	  //	  my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows(),A.nrows()/16),MF(A,X,Y),my_tbb::simple_partitioner());
	  //	  static my_tbb::affinity_partitioner ap;
	  //	  my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows(),A.nrows()/8),MF(A,X,Y),ap);

	  //	  for (int i=0 ; i<100 ; i++)  my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),MF(A,X,Ybis));


	  my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows()),MF(A,X,Y));
	  
	  chronos.stop();
	  chronos.storeResult("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz ter");
	    
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

}

#endif
