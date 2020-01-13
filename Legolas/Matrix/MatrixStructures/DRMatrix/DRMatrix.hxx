/**
 * project  DESCARTES
 *
 * @file     ActualDRMatrix.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRMATRIX_HXX__
#define __LEGOLAS_DRMATRIX_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Matrix/StorageModeTraits/ConstRefTraits.hxx"
#include "Legolas/Matrix/MatrixStructures/Helper/AllFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  template <class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE, class DR_INV, class DR_MULT>
  class DRMatrix:  public BaseMatrix< DRMatrix<D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT> >,
		   public MatrixBasicData<typename D_MATRIX_INTERFACE::Matrix::MatrixElementInterface::DataDriver>
  {
  private:
    typedef BaseMatrix< DRMatrix<D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT> > Base1;
    typedef MatrixBasicData<typename D_MATRIX_INTERFACE::Matrix::MatrixElementInterface::DataDriver> Base2;
  public:
  
    typedef DR_MULT MatrixVectorProduct;
    typedef DR_INV  MatrixVectorInverse;

    typedef typename D_MATRIX_INTERFACE::Matrix::VectorOperand VectorOperand;
    typedef typename MatrixVectorInverse::template Engine<DRMatrix,VectorOperand,VectorOperand > Solver;

    typedef Actual StorageMode;

    typedef typename ConstRefTraits<DRMatrix,StorageMode>::Type StoreType;

    typedef D_MATRIX_INTERFACE DMatrixInterface;
    typedef R_MATRIX_INTERFACE RMatrixInterface;

  private:

    typedef typename DMatrixInterface::MatrixDefinition::Data VirtualDData;
    typedef typename RMatrixInterface::MatrixDefinition::Data VirtualRData;

  public:

    struct DataDriver{
    
      typedef DRMatrix Data;

      static const int level=DMatrixInterface::Matrix::level;
      typedef typename DMatrixInterface::DataDriver::RealType      RealType;
    
      template< class INPUT_DR_MATRIX >
      static inline void copy(const INPUT_DR_MATRIX & inputMatrix, Data & matrix){
	matrix=inputMatrix;
      }
    };

    // Pour compatibilité avec le concept MatrixInterface
    typedef typename D_MATRIX_INTERFACE::Matrix::MatrixElementInterface MatrixElementInterface;
    typedef typename MatrixElementInterface::Copy ElementType;
    typedef ElementType ConstGetElement;

    ElementType sparseGetElement(int i, int j) const{
      ElementType aij;
      const  DRMatrix & block=*this;

      if (i==j){
	aij=block.D().diagonalGetElement(i);
      }
      else{
	aij=block.R().sparseGetElement(i,j);
      }

      return aij;
    }

    void iterateOverBlocksMethod(Legolas::MatrixStream & matrixStream) const {
      Legolas::IterateOverBlocks::apply((*this),matrixStream);
    }


    DRMatrix( void ):D_(VirtualDData()),R_( VirtualRData()){}
    
    DRMatrix(const VirtualDData & vdd,const VirtualRData & vrd):D_(vdd),
								R_(vrd){
      this->setMaxIteration()=this->D().getMaxIteration();
      this->setEpsilon()=this->D().getEpsilon();
      
      this->setRowShape()=this->D().getRowShape();
      this->setColShape()=this->D().getColShape();

    }

    typename DMatrixInterface::View      D( void )       {return D_ ; };
    typename DMatrixInterface::ConstView D( void ) const {return D_ ; };
    typename RMatrixInterface::View      R( void )       {return R_ ; };
    typename RMatrixInterface::ConstView R( void ) const {return R_ ; };
	


    std::string getStructureName( void ) const {
      typedef typename DMatrixInterface::DataDriver DDataDriver;    
      typedef typename RMatrixInterface::DataDriver RDataDriver;    
      std::string result="DRMatrix (D=";
      result+=DDataDriver::name();
      result+=" , R=";
      result+=RDataDriver::name();
      result+=")";
      return result;
    }

    static const int level=DMatrixInterface::Matrix::level;
    
    
    class FilledRows: public AllFilled {
    private:
      typedef typename RMatrixInterface::Matrix::FilledRows RFilledRows;
      typedef typename RFilledRows::FilledCols              RFilledColsInRow;
    public:
	  
      template <class MATRIX>
      FilledRows( const MATRIX & matrix ):AllFilled(matrix.nrows()),
					  rRows_(matrix.R())
      {
	MESSAGE("DR::FilledRows Ctor");
      }
      
      
      class FilledCols{
      public:
	FilledCols( int rowIndex, RFilledRows & rRows): rFilledColsInRow_(rRows.filledColsInRow(rowIndex)),
							rowIndex_(rowIndex),
							jnew_(-1)
	{
	  MESSAGE("DR::FilledRows::FilledCols Ctor");
	}
	
	inline int begin( void ) const {
	  return std::min(rFilledColsInRow_.begin(),rowIndex_);
	}
	
	inline int end( void ) const {
	  return std::max(rFilledColsInRow_.end(),rowIndex_+1);
	}
	
	// Si l'iterateur de R "saute" la diagonale on la rajoute
	// et on s'assure de reprendre l'itération en R après la diagonale.
	inline void next( int & j ) {
	  
	  if ( (j<rowIndex_) ){
	    rFilledColsInRow_.next(j);
	    if (j > rowIndex_){
	      jnew_=j;
	      j=rowIndex_;
	    }
	  }
	  else{
	    if (j>rowIndex_){
	      rFilledColsInRow_.next(j);
	    }
	    else{ // j=rowIndex;
	      if (jnew_==-1){
		rFilledColsInRow_.next(j);
	      }
	      else{
		j=jnew_;
	      }
	    }
	  }

	  return;
	}
	
      private:
	RFilledColsInRow rFilledColsInRow_;
	int rowIndex_;
	int jnew_;
      };
      
      FilledCols filledColsInRow( int rowIndex ) {
	return FilledCols(rowIndex,rRows_);
      }
      
    private:
      RFilledRows rRows_;
    };
      

 


    template <class TRAVERSAL_POLICY>
    struct ElementaryTraversal{
      template <class BLOCK_FILTER,class BLOCK>
      static inline void apply(const BLOCK & block,int i, int j, BLOCK_FILTER & filter){
	typedef typename TRAVERSAL_POLICY:: template Engine<BLOCK_FILTER> ET;
	
	if (i==j){
	  typename DMatrixInterface::Matrix::ConstGetElement dii=block.D().diagonalGetElement(i);
	  ET::applyBlock(dii,filter);
	}
	else{
	  typename RMatrixInterface::Matrix::ConstGetElement rij=block.R().sparseGetElement(i,j);
	  ET::applyBlock(rij,filter);
	}
	  
      }
    };


  private:
    typename DMatrixInterface::Matrix D_;
    typename RMatrixInterface::Matrix R_;
  };

}

#endif
