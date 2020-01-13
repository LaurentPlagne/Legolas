#ifndef __LEGOLAS_MULTILEVELSPARSEMATRIXDEFINITION_HXX__
#define __LEGOLAS_MULTILEVELSPARSEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/Sparse.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/NoneFilled.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

namespace Legolas{

  template <class DATA>
  class TFilledRows: public NoneFilled {
  public:
    
    TFilledRows( const DATA & data ):NoneFilled(data.nrows()),
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
  class TFilledCols: public NoneFilled {
  public:
    
    TFilledCols( const DATA & data ):NoneFilled(data.ncols()),
				    nrows_(data.nrows())
    {
    }
    
    class FilledRows: public NoneFilled{
    public:
      FilledRows( int nrows ):NoneFilled(nrows){}
    };
    
    inline FilledRows filledRowsInCol( int colIndex )
    {
      return FilledRows(nrows_);
    }
  private:
    int nrows_;
  };

  




  template <class REAL_TYPE, int level>
  class MultiLevelSparseMatrixDefinition : public Legolas::DefaultMatrixDefinition<REAL_TYPE>
  {
  public:

    typedef Legolas::Sparse               MatrixStructure;
    typedef REAL_TYPE                     RealType;
    typedef Legolas::MatrixShape<level-1> GetElement;
    typedef Legolas::MatrixShape<level>   Data;  
    
    static inline GetElement sparseGetElement(int i , int j, const Data & data) {
      GetElement result=data.getSubMatrixShape(i,j);
      INFOS("result="<<result.getRowShape());
      throw std::runtime_error("");
      return data.getSubMatrixShape(i,j);
    }

    typedef TFilledRows<Data> FilledRows;
    typedef TFilledCols<Data> FilledCols;
    
  };

  template <class REAL_TYPE>
  class MultiLevelSparseMatrixDefinition<REAL_TYPE,1> : public Legolas::DefaultMatrixDefinition<REAL_TYPE>
  {
  public:

    typedef Legolas::Sparse               MatrixStructure;
    typedef REAL_TYPE                     RealType;
    typedef Legolas::MatrixShape<1>       Data;  
    typedef RealType                      GetElement;
    
    static inline GetElement sparseGetElement(int i , int j, const Data & data) {return 0.0;}

    typedef TFilledRows<Data> FilledRows;
    typedef TFilledCols<Data> FilledCols;
    
  };

}

#endif  
  
