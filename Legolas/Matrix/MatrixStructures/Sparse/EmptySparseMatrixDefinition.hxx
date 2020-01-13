#ifndef __LEGOLAS_EMPTYSPARSEMATRIXDEFINITION_HXX__
#define __LEGOLAS_EMPTYSPARSEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/Sparse.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/NoneFilled.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

namespace Legolas{

  //  template <class ELEMENT_DATA_DRIVER>
  //  class EmptySparseMatrixDefinition : public DefaultPrecision<typename ELEMENT_DATA_DRIVER::RealType> {
  //								     ELEMENT_DATA_DRIVER::level+1> 


  template <class ELEMENT_DATA_DRIVER>
  class EmptySparseMatrixDefinition : public DefaultMatrixDefinition<typename ELEMENT_DATA_DRIVER::RealType>
  {
  public:
  
    typedef Sparse                                 MatrixStructure;
    typedef typename ELEMENT_DATA_DRIVER::RealType RealType;
    typedef typename ELEMENT_DATA_DRIVER::Data     GetElement;
  
  
    //  2 static functions to be defined to model the DENSE_MATRIX_DEFINITION concept 
  
    //  template <class DATA>
    //  static inline GetElement sparseGetElement(int i , int j, const DATA & data) { return zero(data);}
  
    template <class DATA>
    static inline int nrows ( const DATA & data ) {
      return data.nrows();
    }
  
    template <class DATA>
    static inline int ncols ( const DATA & data ) {
      return data.ncols();
    }

    template <class DATA>
    static inline int getRowShape ( const DATA & data ) {
      return data.getRowShape();
    }
  
    template <class DATA>
    static inline int getColShape ( const DATA & data ) {
      return data.getColShape();
    }
  
    //    typedef Legolas::MatrixShape<1> BaseData;

 //    class BaseData : public Legolas::MatrixShape<1>{
//     public:
//       BaseData(int nrows, int ncols):Legolas::MatrixShape<1>(nrows,nrows),nrows_(nrows),ncols_(ncols){}
    
//       int nrows( void ) const {return nrows_;}
//       int ncols( void ) const {return ncols_;}

//     private:
//       int nrows_;
//       int ncols_;
//     };

    template <class DATA>
    class FilledRows: public NoneFilled {
    public:
    
      FilledRows( const DATA & data ):NoneFilled(data.nrows()),
				      ncols_(data.ncols())
      {
      }
    
      class FilledCols: public NoneFilled{
      public:
	FilledCols( int ncols ):NoneFilled(ncols){}
      };
    
      inline FilledCols filledColsInRow( int rowIndex )
      {
	return FilledCols(ncols_);
      }
    private:
      int ncols_;
    };
  
  
    template <class DATA>
    class FilledCols: public NoneFilled {
    public:
    
      FilledCols( const DATA & data ):NoneFilled(data.ncols()),
				      nrows_(data.nrows())
      {
      }
    
      class FilledRows: public NoneFilled{
      public:
	FilledRows( int nrows ):NoneFilled(nrows){}
      };
    
      inline FilledCols filledRowsInCol( int colIndex )
      {
	return FilledRows(nrows_);
      }
    private:
      int nrows_;
    };


  };

}

#endif  
  
