#ifndef __LEGOLAS_TINYBANDEDSYMMETRIC_HXX__
#define __LEGOLAS_TINYBANDEDSYMMETRIC_HXX__

#include "Legolas/Matrix/MatrixStructures/Banded/Banded.hxx"
#include "Legolas/Matrix/MatrixStructures/TinyBandedSymmetric/TinyBandedSymmetricMatrixContainer.hxx"

namespace Legolas{

  class TinyBandedSymmetric{
  public:

    typedef TinyBandedSymmetricMatrixContainer DefaultContainer;

    //------------------------------- TinyBandedSymmetric::Hierarchy --------------------------------------
  
    template <class TBS_ENGINE, class DATA, class CGE, class GE, class EDD>
    class Engine{
    public:

      typedef TBS_ENGINE         TinyBandedSymmetricEngine;
      typedef DATA               Data;
      typedef GE                 GetElement;
      typedef CGE                ConstGetElement;
      typedef EDD                ElementDataDriver;


      class AsBanded : public TinyBandedSymmetricEngine {
      public:


	static inline int linf(const Data & data){
	  return TinyBandedSymmetricEngine::getHalfBandwidth(data);
	}

	static inline int lsup(const Data & data){
	  return TinyBandedSymmetricEngine::getHalfBandwidth(data);
	}

	
	static inline ConstGetElement bandedGetElement(int i, int j, const Data & data){
	  if (i<j) { int t=i ; i=j ; j =t;} //swap
	  return TinyBandedSymmetricEngine::lowerBandedGetElement(i,j,data);
	}

	static inline GetElement bandedGetElement(int i, int j, Data & data){
	  if (i<j) { int t=i ; i=j ; j =t;} //swap
	  return TinyBandedSymmetricEngine::lowerBandedGetElement(i,j,data);
	}
	
      };
    
      class Hierarchy : public Banded::template Engine< AsBanded,DATA,CGE,GE,EDD >::Hierarchy {
      public:
	template <class TINYBANDEDSYMMETRIC_INPUT_MATRIX>
	static inline void elementsCopy( const TINYBANDEDSYMMETRIC_INPUT_MATRIX & inputMatrix, 
					 Data & matrix){
	  
	  const int nrows = inputMatrix.nrows(); 
	  const int hbw = inputMatrix.getHalfBandwidth(); 
	  
	  //	  TinyBandedSymmetricEngine::resize(nrows,hbw,matrix);

	  TinyBandedSymmetricEngine::resize(inputMatrix.getRef(),hbw,matrix);
	
	  for (int i=0 ; i < nrows ; i++){
	    for (int j=std::max(i-hbw,0) ; j <=i  ; j++){
	      ElementDataDriver::copy(inputMatrix.lowerBandedGetElement(i,j),lowerBandedGetElement(i,j,matrix));
	    }
	  }
	}
      };
    };
  };
}

#endif  
