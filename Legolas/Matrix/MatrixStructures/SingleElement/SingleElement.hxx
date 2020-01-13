#ifndef __LEGOLAS_SINGLEELEMENT_HXX__
#define __LEGOLAS_SINGLEELEMENT_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/Sparse.hxx"
#include "Legolas/Matrix/MatrixStructures/Helper/SingleFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/SingleElement/SingleElementMatrixContainer.hxx"

namespace Legolas{

  class SingleElement{
  public:

    typedef SingleElementMatrixContainer DefaultContainer;
  
    //------------------------------- SingleElement::Hierarchy --------------------------------------
  
    template <class SINGLE_ELEMENT_ENGINE, class DATA, class CGE, class GE, class EDD>
    class Engine{
    public:
    
      typedef SINGLE_ELEMENT_ENGINE   SingleElementEngine;
      typedef DATA                    Data;
      typedef GE                      GetElement;
      typedef CGE                     ConstGetElement;
      typedef EDD                     ElementDataDriver;

      class AsSparse : public SingleElementEngine {
      public:

	static bool theSparsityPatternIsConstant( void ) { return false;}

	static inline GetElement sparseGetElement(int i, int j, Data & data){
	  return SingleElementEngine::getSingleElement(i,j,data) ;
	}
      
	static inline ConstGetElement sparseGetElement(int i, int j, const Data & data){
	  return SingleElementEngine::getSingleElement(i,j,data) ;
	}
      
      
	class FilledRows: public SingleFilled {
	public:
	
	  template <class MATRIX>
	  FilledRows( const MATRIX & matrix ):SingleFilled(SingleElementEngine::rowIndex(matrix)),
					      colIndex_(SingleElementEngine::colIndex(matrix))
	  
	  {
	    MESSAGE("SingleElementAsSparse::FilledRows Ctor");
	  }
	
	
	  class FilledCols: public SingleFilled{
	  public:
	    FilledCols( int colIndex ):SingleFilled(colIndex){
	      MESSAGE("SingleElementAsSparse::FilledRows::FilledCols Ctor");
	    };
	  };
	
	  inline FilledCols filledColsInRow( int rowIndex )
	  {
	    return FilledCols(colIndex_);
	  }
	
	private:
	  int colIndex_;
	};
      

	class FilledCols: public SingleFilled {
	public:
	
	  template <class MATRIX>
	  FilledCols( const MATRIX & matrix ):SingleFilled(SingleElementEngine::colIndex(matrix.getRef())),
					      rowIndex_(SingleElementEngine::rowIndex(matrix.getRef()))
	  {
	    MESSAGE("SingleElementAsSparse::FilledCols Ctor");
	  }
	
	  class FilledRows: public SingleFilled{
	  public:
	    FilledRows( int colIndex ):SingleFilled(colIndex){
	      MESSAGE("SingleElementAsSparse::FilledCols::FilledRows Ctor");
	    };
	  };
	
	  inline FilledRows filledColsInRow( int colIndex )
	  {
	    return FilledRows(rowIndex_);
	  }
	private:
	  int rowIndex_;
	
	};
      
      
      };


    
      class Hierarchy : public Sparse::template Engine< AsSparse,DATA,CGE,GE,EDD >::Hierarchy {
      public:
	template <class SINGLE_ELEMENT_INPUT_MATRIX>
	static inline void elementsCopy( const SINGLE_ELEMENT_INPUT_MATRIX & inputMatrix, 
					 Data & matrix){
	  int nrows = inputMatrix.nrows(); 
	  int ncols = inputMatrix.ncols(); 
	
	  resize(nrows,ncols,matrix);
	
	  int rowIndex=inputMatrix.rowIndex(); 
	  int colIndex=inputMatrix.colIndex(); 
	
	  ElementDataDriver::copy(inputMatrix.getSingleElement(rowIndex,colIndex),getSingleElement(rowIndex,colIndex,matrix));
	  //	  ElementDataDriver::copy(inputMatrix.zero(),zero(matrix));

	}
      };

    };

  };

}

#endif

  
