#ifndef __LEGOLAS_SPARSE_HXX__
#define __LEGOLAS_SPARSE_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/SparseVectorMapContainer.hxx"

namespace Legolas{

  class Sparse{
  public:

    typedef SparseVectorMapContainer DefaultContainer;
  
    //------------------------------- Sparse::Hierarchy --------------------------------------
  
    template <class SPARSE_ENGINE, class DATA, class CGE, class GE, class EDD>
    class Engine{
    public:
    
      typedef SPARSE_ENGINE   SparseEngine;
      typedef DATA            Data;
      typedef GE              GetElement;
      typedef CGE             ConstGetElement;
      typedef EDD             ElementDataDriver;

      class Hierarchy : public SparseEngine {
      public:
	static std::string name( void ) { return "Sparse" ; }

	typedef DATA Data;

	template <class MATRIX_AXIS_ITERATOR>
	static bool isInSequence(const MATRIX_AXIS_ITERATOR & axis, const int & axisIndex){
	  bool result=false;
	  int index=axis.begin();
	  while (index<axisIndex && index!=axis.end() ) axis.next(index);
	  if (index==axisIndex && index!=axis.end() ) result=true;
	  return result;
	}
      
	static bool thisRowIsNotEmpty(const Data & matrix, const int & rowIndex){
	  typedef typename SparseEngine::FilledRows FilledRows;
	  FilledRows rows(matrix);
	  return isInSequence(rows,rowIndex);
	}

	static bool thisColInRowIsNotEmpty(const Data & matrix, const int & rowIndex, const int & colIndex){
	  typedef typename SparseEngine::FilledRows FilledRows;
	  FilledRows rows(matrix);
	  typename FilledRows::FilledCols cols=rows.filledColsInRow(rowIndex);
	  return isInSequence(cols,colIndex);
	}
      
	static bool thisElementExists(const int rowIndex, const int colIndex, const Data & matrix){
	  bool result=false;
	  if (thisRowIsNotEmpty(matrix,rowIndex)){
	    result=thisColInRowIsNotEmpty(matrix,rowIndex,colIndex);
	  }
	  return result;
	}

      
	
	template <class SPARSE_INPUT_MATRIX>
	static inline void elementsCopy( const SPARSE_INPUT_MATRIX & inputMatrix, 
					 Data & matrix){

	  Hierarchy::resize(inputMatrix.getRef(),matrix);
	
	  typedef typename SPARSE_INPUT_MATRIX::FilledRows FilledRows;
	  FilledRows rows(inputMatrix);
	
	  for (int i=rows.begin() ; i!=rows.end() ; rows.next(i)){
	    typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	    for (int j=cols.begin() ; j!=cols.end() ; cols.next(j)){
	      ElementDataDriver::copy(inputMatrix.sparseGetElement(i,j),SparseEngine::sparseGetElement(i,j,matrix));
	    }
	  }
	
	}
      };
    };
  };

}

#endif

  
