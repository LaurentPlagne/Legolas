#ifndef __LEGOLAS_DENSE_HXX__
#define __LEGOLAS_DENSE_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/Sparse.hxx"
#include "Legolas/Matrix/MatrixStructures/Helper/AllFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/Dense/DenseF77MatrixContainer.hxx"

namespace Legolas{

  class Dense{
  public:
  
    typedef DenseF77MatrixContainer DefaultContainer;
  
    //------------------------------- Dense::Hierarchy --------------------------------------
  
    template <class DENSE_ENGINE, class DATA, class CGE, class GE, class EDD>
    class Engine{
    public:

      typedef DENSE_ENGINE   DenseEngine;
      typedef DATA           Data;
      typedef GE             GetElement;
      typedef CGE            ConstGetElement;
      typedef EDD            ElementDataDriver;

      class AsSparse : public DENSE_ENGINE {
      public:

	static bool theSparsityPatternIsConstant( void ) { return false;}

	static inline GetElement sparseGetElement(int i, int j, Data & data){
	  return DenseEngine::getElement(i,j,data) ;
	}
      
	static inline ConstGetElement sparseGetElement(int i, int j, const Data & data){
	  return DenseEngine::getElement(i,j,data) ;
	}
      
      
	class FilledRows: public AllFilled {
	private:
	  int ncols_;
	public:
	  template <class MATRIX>
	  FilledRows( const MATRIX & matrix ):AllFilled(matrix.nrows()),
					      ncols_(matrix.ncols())
	  {
	    MESSAGE("DenseAsSparse::FilledRows Ctor");
	  }
	
	  class FilledCols: public AllFilled{
	  public:
	    FilledCols( int size ):AllFilled(size){
	      MESSAGE("DenseAsSparse::FilledRows::FilledCols Ctor");
	    };
	  };
	
	  inline FilledCols filledColsInRow( int rowIndex )
	  {
	    return FilledCols(ncols_);
	  }
	
	};
      
	class FilledCols: public AllFilled {
	private:
	  int nrows_;
	public:

	  template <class MATRIX>
	  FilledCols( const MATRIX & matrix ):AllFilled(matrix.ncols()),
					      nrows_(matrix.nrows())
	  {
	    MESSAGE("DenseAsSparse::FilledCols Ctor");
	  }
	
	  class FilledRows: public AllFilled{
	  public:
	    FilledRows( int size ):AllFilled(size){
	      MESSAGE("DenseAsSparse::FilledCols::FilledRows Ctor");
	    };
	  };
	
	  inline FilledRows filledRowsInCol( int rowIndex )
	  {
	    return FilledRows(nrows_);
	  }
	
	};
      
      };

    
      class Hierarchy : public Sparse::template Engine< AsSparse,DATA,CGE,GE,EDD>::Hierarchy {
      public:
	static std::string name( void ) { return "Dense" ; }

	typedef typename Sparse::template Engine< AsSparse,DATA,CGE,GE,EDD>::Hierarchy Base;

	template <class DENSE_INPUT_MATRIX>
	static inline void elementsCopy( const DENSE_INPUT_MATRIX & inputMatrix, 
					 Data & matrix){
	
	  int nrows=inputMatrix.nrows(); 
	  int ncols=inputMatrix.ncols(); 

	  //inputMatrix should inherit from MatrixShape<>

	  Hierarchy::resize(inputMatrix.getRef(),matrix);
	
	  for (int i=0 ; i < nrows ; i++){
	    for (int j=0 ; j < ncols ; j++){

	      ElementDataDriver::copy(inputMatrix(i,j),Base::getElement(i,j,matrix));
	    
	    }
	  }
	
	}
      };

    };

  };
    
}

#endif

  
