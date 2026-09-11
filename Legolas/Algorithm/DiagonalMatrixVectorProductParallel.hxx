#pragma once

#include "UTILITES.hxx"
#include "X86Timer.hxx"
#include "Parallel.hxx"

namespace Legolas{

  // Y=A*X

  class DiagonalMatrixVectorProductParallel{
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
	
	inline void operator()(const Legolas::blocked_range<int> & r) const {
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
	  typedef MultFunctor<MATRIX,VECTOR,VECTOR_INOUT> MF;

	  X86Timer chronos;
	  chronos.start();

	  Legolas::parallel_for(Legolas::blocked_range<int>(0,A.nrows()),MF(A,X,Y));
	  
	  chronos.stop();
	  chronos.storeResult("Legolas::parallel_for");
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
	    ASSIGN_MODE::apply(Y[0],transpose(A.diagonalGetElement(0))*X[0]);
	  }
	  else{
	    for (int i=0 ; i < A.nrows() ; i++){
	      ASSIGN_MODE::apply(Y[i],transpose(A.diagonalGetElement(i))*X[i]);
	    }
	  }
	
	  return ;
	}      
      };
    };
  };

  class DiagonalMatrixVectorProductParallelBis{
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
	
	inline void operator()(const Legolas::blocked_range<int> & r) const {
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
	  typedef MultFunctor<MATRIX,VECTOR,VECTOR_INOUT> MF;

	  X86Timer chronos;
	  chronos.start();

	  Legolas::parallel_for(Legolas::blocked_range<int>(0,A.nrows()),MF(A,X,Y));
	  
	  chronos.stop();
	  chronos.storeResult("Legolas::parallel_for");
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
	    ASSIGN_MODE::apply(Y[0],transpose(A.diagonalGetElement(0))*X[0]);
	  }
	  else{
	    for (int i=0 ; i < A.nrows() ; i++){
	      ASSIGN_MODE::apply(Y[i],transpose(A.diagonalGetElement(i))*X[i]);
	    }
	  }
	
	  return ;
	}      
      };
    };
  };

}
