/**
 * project  DESCARTES
 *
 * @file     AMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_NULLMATRIXDEFINITION_HXX__
#define __LEGOLAS_NULLMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/NoneFilled.hxx"
#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "Legolas/Matrix/Helper/VoidObject.hxx"

#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

namespace Legolas{

  template <class REAL_TYPE, int LEVEL>
  class NullMatrixDefinition : 
    public NullMatrixDefinition<REAL_TYPE,1>
  {
  public:
    // Types that must be defined to model the MATRIX_DEFINITION concept
  
    typedef Legolas::Sparse                          MatrixStructure;
    typedef REAL_TYPE                                RealType;
    typedef typename NullMatrixDefinition<REAL_TYPE,LEVEL-1>::Data  GetElement;
    
    typedef Legolas::MatrixShape<LEVEL>    Data;
    
    static inline GetElement sparseGetElement( int i , int j, const Data & data) {
      return data.getSubMatrixShape(i,j);
    }
    
  };


  template <class REAL_TYPE>
  class NullMatrixDefinition<REAL_TYPE,1> : public DefaultPrecision<REAL_TYPE>{
  public:
    // Types that must be defined to model the MATRIX_DEFINITION concept
  
    typedef Legolas::Sparse                          MatrixStructure;
    typedef REAL_TYPE                                RealType;
    typedef REAL_TYPE                                GetElement;
    
    typedef Legolas::MatrixShape<1>                  Data;
    
    
    static inline GetElement sparseGetElement( int i , int j, const Data & data) {
      return 0.0;
    }

    class FilledRows : public Legolas::NoneFilled {
    public:
      
      template <class MATRIX>
      FilledRows( const MATRIX & matrix ):Legolas::NoneFilled(matrix.nrows()),
					  ncols_(matrix.ncols())
      {
	MESSAGE("NullMatrix::FilledRows Ctor");
      }
      
      class FilledCols: public NoneFilled {
      public:
	FilledCols( int ncols  ):Legolas::NoneFilled(ncols)
	{
	  MESSAGE("NullMatrix::FilledRows::FilledCols Ctor");
	}
      };
      
      FilledCols filledColsInRow( int rowIndex ) {
	return ncols_;
      }
    private:
      int ncols_;
    };


    class FilledCols: public Legolas::NoneFilled {
    public:
      
      template <class MATRIX>
      FilledCols( const MATRIX & matrix ):Legolas::NoneFilled(matrix.ncols()),
					  nrows_(matrix.nrowss())
      {
	MESSAGE("NullMatrix::FilledCols Ctor");
      }
      
      class FilledRows: public NoneFilled {
      public:
	FilledRows( int nrows  ):NoneFilled(nrows)
	{
	  MESSAGE("NullMatrix::FilledCols::FilledRows Ctor");
	}
      };
      
      FilledRows filledRowsInCol( int colIndex ) {
	return nrows_;
      }
      
    private:
      int nrows_;
    };


  };

}

#endif
    

									     

