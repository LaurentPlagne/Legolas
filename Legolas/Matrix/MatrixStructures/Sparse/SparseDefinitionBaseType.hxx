#ifndef __LEGOLAS_SPARSEDEFINITIONBASETYPE_HXX__
#define __LEGOLAS_SPARSEDEFINITIONBASETYPE_HXX__

namespace Legolas{

  class SparseDefinitionBaseType{
  public:
    class BaseData{
    public:
      BaseData(int nrows, int ncols):nrows_(nrows),ncols_(ncols){}
    
      int nrows( void ) const {return nrows_;}
      int ncols( void ) const {return nrows_;}

    private:
      int nrows_;
      int ncols_;
    };

    template <class DATA>
    class BaseFilledRows: public NoneFilled {
    public:
    
      BaseFilledRows( const DATA & data ):NoneFilled(data.nrows()),
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
	INFOS("ICI data="<<data);
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
