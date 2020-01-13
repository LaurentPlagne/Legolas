#ifndef __LEGOLAS_TRIDIAGONAL_HXX__
#define __LEGOLAS_TRIDIAGONAL_HXX__

#include "Legolas/Matrix/MatrixStructures/Banded/Banded.hxx"
#include "Legolas/Matrix/MatrixStructures/TriDiagonal/TriDiagonalMatrixContainer.hxx"

namespace Legolas{

  class TriDiagonal{
  public:

    typedef TriDiagonalMatrixContainer DefaultContainer;

    //------------------------------- TriDiagonal::Hierarchy --------------------------------------
  
    template <class TRIDIAGONAL_ENGINE, class DATA, class CGE, class GE, class EDD>
    class Engine{
    public:

      typedef TRIDIAGONAL_ENGINE TriDiagonalEngine;
      typedef DATA               Data;
      typedef GE                 GetElement;
      typedef CGE                ConstGetElement;
      typedef EDD                ElementDataDriver;


      class AsBanded : public TriDiagonalEngine {
      public:
	static inline ConstGetElement bandedGetElement(int i, int j, const Data & data){
      
	  if (i==j){ 
	    return TriDiagonalEngine::diagonalGetElement(i,data);
	  }
	  else{
	    if (i==j+1){ 
	      return TriDiagonalEngine::lowerDiagonalGetElement(i,data);
	    }
	    else{
	      if (i==j-1){ 
		return TriDiagonalEngine::upperDiagonalGetElement(i,data);
	      }
	      else{
		INFOS("TriDiagonalAsBanded Index Out of Band");
		throw std::runtime_error("TriDiagonalAsBanded Index Out of Band");
		return TriDiagonalEngine::upperDiagonalGetElement(i,data);
	      }
	    }
	  }
	}
      
	static inline GetElement bandedGetElement(int i, int j, Data & data){
	
	  if (i==j){ 
	    return TriDiagonalEngine::diagonalGetElement(i,data);
	  }
	  else{
	    if (i==j+1){ 
	      return TriDiagonalEngine::lowerDiagonalGetElement(i,data);
	    }
	    else{
	      if (i==j-1){ 
		return TriDiagonalEngine::upperDiagonalGetElement(i,data);
	      }
	      else{ 
		INFOS("This accessor should not be used out of the bandwidth");
		throw std::runtime_error("This accessor should not be used out of the bandwidth");
		//		return TriDiagonalEngine::zero(data) ;
	      }
	    }
	  }
	}
      
      
// 	static inline ConstGetElement zero(const Data & data){
// 	  return TriDiagonalEngine::zero(data);
// 	}
      
// 	static inline GetElement zero(Data & data){
// 	  return TriDiagonalEngine::zero(data);
// 	}
      
	static inline int linf(const Data & data){
	  return 1;
	}
      
	static inline int lsup(const Data & data){
	  return 1;
	}
      
      };
    
      class Hierarchy : public Banded::template Engine< AsBanded,DATA,CGE,GE,EDD >::Hierarchy {
      public:
	template <class TRIDIAGONAL_INPUT_MATRIX>
	static inline void elementsCopy( const TRIDIAGONAL_INPUT_MATRIX & inputMatrix, 
					 Data & matrix){
	
	  int nrows = inputMatrix.nrows(); 
	
	  resize(nrows,matrix);
	
	  ElementDataDriver::copy(inputMatrix.diagonalGetElement(0),diagonalGetElement(0,matrix));
	  ElementDataDriver::copy(inputMatrix.upperDiagonalGetElement(0),upperDiagonalGetElement(0,matrix));
	
	  for (int i=1 ; i < nrows-1 ; i++){
	  
	    ElementDataDriver::copy(inputMatrix.diagonalGetElement(i),diagonalGetElement(i,matrix));
	    ElementDataDriver::copy(inputMatrix.upperDiagonalGetElement(i),upperDiagonalGetElement(i,matrix));
	    ElementDataDriver::copy(inputMatrix.lowerDiagonalGetElement(i),lowerDiagonalGetElement(i,matrix));
	  
	  }
	
	  ElementDataDriver::copy(inputMatrix.diagonalGetElement(nrows-1),diagonalGetElement(nrows-1,matrix));
	  ElementDataDriver::copy(inputMatrix.lowerDiagonalGetElement(nrows-1),lowerDiagonalGetElement(nrows-1,matrix));
	
	  //	  ElementDataDriver::copy(inputMatrix.zero(),zero(matrix));
	
	}
      
      };
    
    };

  };


}

#endif  
