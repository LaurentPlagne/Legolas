/**
 * project  DESCARTES
 *
 * @file     NullMatrix.hxx
 *
 * @author Laurent PLAGNE
 * @date   2009
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - 2009
 */
#ifndef __LEGOLAS_NULLMATRIX_HXX__
#define __LEGOLAS_NULLMATRIX_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Matrix/StorageModeTraits/ConstRefTraits.hxx"
#include "Legolas/Matrix/MatrixStructures/Helper/AllFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/NoneFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"
#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"

namespace Legolas{

  template <class REAL_TYPE, int LEVEL>
  class NullMatrix : public BaseMatrix< NullMatrix<REAL_TYPE,LEVEL> >,
		     public MatrixBasicData< typename NullMatrix<REAL_TYPE,LEVEL-1>::DataDriver >
  {
  public:
	      
    typedef MatrixBasicData< typename NullMatrix<REAL_TYPE,LEVEL-1>::DataDriver > Base;

    typedef Legolas::SparseMatrixVectorProduct                   MatrixVectorProduct;
    typedef Legolas::SparseGaussSeidel                           MatrixVectorInverse;
    
    typedef Virtual StorageMode;
	      
    typedef typename ConstRefTraits<NullMatrix,StorageMode>::Type StoreType;
	      
  public:
	      
    struct DataDriver{
		
      typedef NullMatrix Data;
      
      static const int level=LEVEL;
      typedef REAL_TYPE RealType;
      
      template< class INPUT_NULL_MATRIX >
      static inline void copy(const INPUT_NULL_MATRIX & inputMatrix, Data & matrix){
	//	matrix=inputMatrix;
      }
    };
	      
	      
    NullMatrix(const MatrixShape<LEVEL> & ms):Base(ms){}

    NullMatrix( void ){}

    std::string getStructureName( void ) const { return "NullMatrix";}

    static const int level=LEVEL;
    
    class FilledRows: public Legolas::NoneFilled {
    public:
      
      template <class MATRIX>
      FilledRows( const MATRIX & matrix ):NoneFilled(matrix.nrows()),
					  ncols_(matrix.ncols())
      {
	MESSAGE("NullMatrix::FilledRows Ctor");
      }
      
      class FilledCols: public NoneFilled {
      public:
	FilledCols( int ncols  ):NoneFilled(ncols)
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
      

  };


  template <class REAL_TYPE> 
  class NullMatrix<REAL_TYPE,1>:  public BaseMatrix< NullMatrix<REAL_TYPE,1> >,
				  public MatrixBasicData< typename Legolas::RealDataDriver<REAL_TYPE> >
  {
  public:

    typedef MatrixBasicData< typename Legolas::RealDataDriver<REAL_TYPE> > Base;
	      
    typedef Legolas::SparseMatrixVectorProduct                  MatrixVectorProduct;
    typedef Legolas::SparseGaussSeidel                          MatrixVectorInverse;
    
    typedef Virtual StorageMode;
	      
    typedef typename ConstRefTraits<NullMatrix,StorageMode>::Type StoreType;
	      
  public:
	      
    struct DataDriver{
		
      typedef NullMatrix Data;
      
      static const int level=1;
      typedef REAL_TYPE RealType;
      
      template< class INPUT_NULL_MATRIX >
      static inline void copy(const INPUT_NULL_MATRIX & inputMatrix, Data & matrix){
	//	matrix=inputMatrix;
      }
    };
	      
    NullMatrix(const MatrixShape<1> & ms):Base(ms){}
	      
    NullMatrix( void ){}

    std::string getStructureName( void ) const { return "NullMatrix_1";}

    static const int level=1;
    
    class FilledRows: public Legolas::NoneFilled {
    public:
      
      template <class MATRIX>
      FilledRows( const MATRIX & matrix ):NoneFilled(matrix.nrows()),
					  ncols_(matrix.ncols())
      {
	MESSAGE("NullMatrix::FilledRows Ctor");
      }
      
      class FilledCols: public NoneFilled {
      public:
	FilledCols( int ncols  ):NoneFilled(ncols)
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
      

  };

}

#endif
