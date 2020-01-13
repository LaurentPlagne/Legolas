#ifndef __LEGOLAS_BANDED_HXX__
#define __LEGOLAS_BANDED_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/Sparse.hxx"
#include "Legolas/Matrix/MatrixStructures/Helper/AllFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/Banded/BandedCCompactMatrixContainer.hxx"

namespace Legolas{

  class Banded{
  public:

    typedef BandedCCompactMatrixContainer DefaultContainer;
    
    //------------------------------- Banded::Hierarchy --------------------------------------
  
    template <class BANDED_ENGINE, class DATA, class CGE, class GE, class EDD>
    class Engine{
    public:

      typedef BANDED_ENGINE   BandedEngine;
      typedef DATA            Data;
      typedef GE              GetElement;
      typedef CGE             ConstGetElement;
      typedef EDD             ElementDataDriver;

      //      static const int level=ElementDataDriver::level+1;
      typedef typename Data::Shape Shape;

      class AsSparse : public BANDED_ENGINE {
      public:

	static bool theSparsityPatternIsConstant( void ) { return true;}

// 	static inline Shape getColShape(const Data & data){ 
// 	  return data.getRowShape();
// 	  //	  return BandedEngine::getRowShape(data); // Banded Matrix are square matrix
// 	}

// 	static inline int ncols(const Data & data){ 
// 	  return BandedEngine::nrows(data); // Banded Matrix are square matrix
// 	}

	static inline GetElement sparseGetElement(int i, int j, Data & data){
	  return BandedEngine::bandedGetElement(i,j,data) ;
	}
      
	static inline ConstGetElement sparseGetElement(int i, int j, const Data & data){
	  return BandedEngine::bandedGetElement(i,j,data) ;
	}
      
	class FilledRows: public AllFilled {
	public:
	  
	  //	  template <class MATRIX>
	  //	  FilledRows( const MATRIX & matrix ):AllFilled(BandedEngine::nrows(matrix)),
	  template <class MATRIX>
	  FilledRows( const MATRIX & matrix ):AllFilled(matrix.nrows()),
					      linf_(BandedEngine::linf(matrix)),
					      lsup_(BandedEngine::lsup(matrix))
	  {
	    MESSAGE("BandedAsSparse::FilledRows Ctor");
	  }
	
	
	  class FilledCols{
	  public:
	    FilledCols( int rowIndex, int lsup, int linf, int nrows ):begin_(std::max(rowIndex-linf,0)),
								      end_(std::min(rowIndex+lsup+1,nrows))
	    {
	      MESSAGE("BandedAsSparse::FilledRows::FilledCols Ctor");
	    }
	  
	    inline int begin( void ) const {
	      return begin_;
	    }
	  
	    inline int end( void ) const {
	      return end_;
	    }
	    
	    inline void next( int & j ) const{
	      j++;
	      return;
	    }
	  
	  private:
	    int begin_;
	    int end_;
	  };
	
	  FilledCols filledColsInRow( int rowIndex ){
	    return FilledCols(rowIndex,lsup_,linf_,size());
	  }
	
	private:
	  int linf_;
	  int lsup_;
	};
      
      
	class FilledCols: public AllFilled {
	public:
	
	
	  //	  template <class MATRIX>
	  //	  FilledCols( const MATRIX & matrix ):AllFilled(BandedEngine::nrows(matrix)),
	  template <class MATRIX>
	  FilledCols( const MATRIX & matrix ):AllFilled(matrix.nrows()),
					      linf_(BandedEngine::lsup(matrix)),
					      lsup_(BandedEngine::linf(matrix))
	  {
	    MESSAGE("BandedAsSparse::FilledCols Ctor");
	  }
	
	
	  class FilledRows{
	  public:
	    FilledRows( int rowIndex, int lsup, int linf, int nrows ):begin_(std::max(rowIndex-linf,0)),
								      end_(std::min(rowIndex+lsup+1,nrows))
	    {
	      MESSAGE("BandedAsSparse::FilledCols::FilledRows Ctor");
	    }
	  
	    inline int begin( void ) const {
	      return begin_;
	    }
	  
	    inline int end( void ) const {
	      return end_;
	    }
	  
	    inline void next( int & j ) const{
	      j++;
	      return;
	    }
	  
	  private:
	    int begin_;
	    int end_;
	  };
	
	  FilledRows filledRowsInCol( int rowIndex ){
	    return FilledRows(rowIndex,lsup_,linf_,size());
	  }
	
	private:
	  int linf_;
	  int lsup_;
	};
      
      };
    
    
      class Hierarchy :  public Sparse::template Engine< AsSparse,DATA,CGE,GE,EDD >::Hierarchy {
      public:
        typedef typename Sparse::template Engine< AsSparse,DATA,CGE,GE,EDD >::Hierarchy Base;
	static std::string name( void ) { return "Banded" ; }

	template <class BANDED_INPUT_MATRIX>
	static inline void elementsCopy( const BANDED_INPUT_MATRIX & inputMatrix, 
					 Data & matrix){
	
	  int nrows = inputMatrix.nrows(); 
	  int linf  = inputMatrix.linf(); // Nb of non zero elements below the diagonal
	  int lsup  = inputMatrix.lsup(); // Nb of non zero elements above the diagonal
	
	  Hierarchy::resize(inputMatrix.getRef(),linf,lsup,matrix);
	
	  for (int i=0 ; i < nrows ; i++){
	    for (int j=std::max(i-linf,0) ; j < std::min(i+lsup+1,nrows) ; j++){
	      ElementDataDriver::copy(inputMatrix.bandedGetElement(i,j),Base::bandedGetElement(i,j,matrix));
	    }
	  }

	}
      };
      
    };
  
  };

}

#endif

  
