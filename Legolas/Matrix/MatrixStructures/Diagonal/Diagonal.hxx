#ifndef __LEGOLAS_DIAGONAL_HXX__
#define __LEGOLAS_DIAGONAL_HXX__

#include "Legolas/Matrix/MatrixStructures/Banded/Banded.hxx"
#include "Legolas/Matrix/MatrixStructures/Diagonal/DiagonalMatrixContainer.hxx"

namespace Legolas{

  class Diagonal{
  public:

    typedef DiagonalMatrixContainer DefaultContainer;

    //------------------------------- Diagonal::Hierarchy --------------------------------------
  
    template <class DIAGONAL_ENGINE, class DATA, class CGE, class GE, class EDD>
    class Engine{
    public:
    
      typedef DIAGONAL_ENGINE   DiagonalEngine;
      typedef DATA              Data;
      typedef GE                GetElement;
      typedef CGE               ConstGetElement;
      typedef EDD               ElementDataDriver;


      class AsBanded : public DiagonalEngine {
      public:
	static inline GetElement bandedGetElement(int i, int j, Data & data){
	  return  DiagonalEngine::diagonalGetElement(i,data); // i==j is assumed
	}
      
	static inline ConstGetElement bandedGetElement(int i, int j, const Data & data){
	  return  DiagonalEngine::diagonalGetElement(i,data); // i==j is assumed
	}
      
// 	static inline ConstGetElement zero(const Data & data){
// 	  return  DiagonalEngine::zero(data);     
// 	}
      
// 	static inline GetElement zero(Data & data){
// 	  return  DiagonalEngine::zero(data);     
// 	}
      
	static inline int linf(const Data & data){
	  return 0;
	}
      
	static inline int lsup(const Data & data){
	  return 0;
	}
      
	//       static inline int nrows(const Data & data){
	// 	return DiagonalEngine::nrows(data);
	//       }
      
      };

    
      class Hierarchy : public Banded::template Engine< AsBanded,DATA,CGE,GE,EDD>::Hierarchy {
      public:
      

	typedef typename Banded::template Engine< AsBanded,DATA,CGE,GE,EDD>::Hierarchy Base;
	typedef DATA Data;
	typedef GE GetElement;

	static std::string name( void ) { return "Diagonal" ; }

// 	template <class DIAGONAL_INPUT_MATRIX>
// 	static inline void elementsCopy( const DIAGONAL_INPUT_MATRIX & inputMatrix, 
// 					 Data & matrix){
// 	  int nrows = inputMatrix.nrows(); 
	
// 	  resize(nrows,matrix);
	
// 	  for (int i=0 ; i < nrows ; i++){

	  
// 	    ElementDataDriver::copy(inputMatrix.diagonalGetElement(i),diagonalGetElement(i,matrix));
	  
// 	  }
// 	}

	template <class DIAGONAL_INPUT_MATRIX>
	static inline void elementsCopy( const DIAGONAL_INPUT_MATRIX & inputMatrix, 
					 Data & matrix){
	  int nrows = inputMatrix.nrows(); 
	
	  Hierarchy::resize(inputMatrix.getRef(),matrix);
	  //	  resize(nrows,matrix);
	  
	  for (int i=0 ; i < nrows ; i++){

	    //	    typename ElementDataDriver::Data eCopy=2.0;
	    //	    diagonalGetElement(i,matrix)=2.0;
	    //	    inputMatrix.diagonalGetElement(i)=2.0;

	    //	    typename ElementDataDriver::Data eCopy=inputMatrix.diagonalGetElement(i);
	    //	    matrix.push_back(inputMatrix.diagonalGetElement(i));
	    
	    ElementDataDriver::copy(inputMatrix.diagonalGetElement(i),Base::diagonalGetElement(i,matrix));
	  
	  }
	}
      
      };
    

    };

  };

}

#endif

  
