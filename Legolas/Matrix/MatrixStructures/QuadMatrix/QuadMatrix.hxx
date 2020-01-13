/**
 * project  DESCARTES
 *
 * @file     QuadMatrix.hxx
 *
 * @author Laurent PLAGNE
 * @date   2009
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - 2009
 */
#ifndef __LEGOLAS_QUADMATRIX_HXX__
#define __LEGOLAS_QUADMATRIX_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Matrix/StorageModeTraits/ConstRefTraits.hxx"
#include "Legolas/Matrix/MatrixStructures/Helper/AllFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  template <class MI_00, 
	    class MI_01, 
	    class MI_10, 
	    class MI_11, 
	    class QUAD_INV, class QUAD_MULT>
  class QuadMatrix:  public BaseMatrix< QuadMatrix<MI_00,MI_01,MI_10,MI_11,QUAD_INV,QUAD_MULT> >,
		     public MatrixBasicData<typename MI_00::DataDriver>
  {
  public:
	      
    typedef QUAD_MULT MatrixVectorProduct;
    typedef QUAD_INV  MatrixVectorInverse;
	      
    typedef Actual StorageMode;
	      
    typedef typename ConstRefTraits<QuadMatrix,StorageMode>::Type StoreType;
	      
    typedef MI_00 MatrixInterface_00;
    typedef MI_01 MatrixInterface_01;
    typedef MI_10 MatrixInterface_10;
    typedef MI_11 MatrixInterface_11;

  private:

    typedef typename MatrixInterface_00::MatrixDefinition::Data VirtualData_00;
    typedef typename MatrixInterface_01::MatrixDefinition::Data VirtualData_01;
    typedef typename MatrixInterface_10::MatrixDefinition::Data VirtualData_10;
    typedef typename MatrixInterface_11::MatrixDefinition::Data VirtualData_11;

  public:
	      
    struct DataDriver{
		
      typedef QuadMatrix Data;
		
      static const int level=MatrixInterface_00::Matrix::level+1;
      typedef typename MatrixInterface_00::DataDriver::RealType    RealType;
		
      template< class INPUT_QUAD_MATRIX >
      static inline void copy(const INPUT_QUAD_MATRIX & inputMatrix, Data & matrix){
	matrix=inputMatrix;
      }
    };

    static const int level=MatrixInterface_00::Matrix::level+1;
//     typedef typename MatrixInterface_00::Matrix::RealType RealType;
//     typedef RectangularMultiVector<RealType,level> VectorOperand;
//     typedef typename MatrixVectorInverse::template Engine<QuadMatrix,VectorOperand,VectorOperand > Solver;

//     // Pour compatibilité avec le concept MatrixInterface
//     typedef MatrixInterface_00 MatrixElementInterface;
//     typedef typename MatrixElementInterface::Copy ElementType;
    
	      
    QuadMatrix( void ):M00_( VirtualData_00()),
		       M01_( VirtualData_01()),
		       M10_( VirtualData_10()),
		       M11_( VirtualData_11()){}

	      
    QuadMatrix(const VirtualData_00 & vd00,
	       const VirtualData_01 & vd01,
	       const VirtualData_10 & vd10,
	       const VirtualData_11 & vd11):M00_(vd00),
					    M01_(vd01),
					    M10_(vd10),
					    M11_(vd11){
      

            this->setMaxIteration()=this->M00().getMaxIteration();
	    this->setEpsilon()=this->M00().getEpsilon();

	    this->setRowShape().appendElement(this->M00().getRowShape());
	    this->setRowShape().appendElement(this->M10().getRowShape());	    


	    this->setColShape().appendElement(this->M00().getColShape());
	    this->setColShape().appendElement(this->M01().getColShape());
	    
    }
	      
    typename MatrixInterface_00::View      M00( void )       {return M00_ ; };
    typename MatrixInterface_01::View      M01( void )       {return M01_ ; };
    typename MatrixInterface_10::View      M10( void )       {return M10_ ; };
    typename MatrixInterface_11::View      M11( void )       {return M11_ ; };

    typename MatrixInterface_00::ConstView M00( void ) const {return M00_ ; };
    typename MatrixInterface_01::ConstView M01( void ) const {return M01_ ; };
    typename MatrixInterface_10::ConstView M10( void ) const {return M10_ ; };
    typename MatrixInterface_11::ConstView M11( void ) const {return M11_ ; };


    std::string getStructureName( void ) const {
      typedef typename MatrixInterface_00::DataDriver DataDriver_00;    
      typedef typename MatrixInterface_01::DataDriver DataDriver_01;    
      typedef typename MatrixInterface_10::DataDriver DataDriver_10;    
      typedef typename MatrixInterface_11::DataDriver DataDriver_11;    
		
      std::string result="QuadMatrix (";
      result+=" M00=" ; result+=DataDriver_00::name(); result+=",";
      result+=" M01=" ; result+=DataDriver_01::name(); result+=",";
      result+=" M10=" ; result+=DataDriver_10::name(); result+=",";
      result+=" M11=" ; result+=DataDriver_11::name(); result+=")";
      return result;
    }

    //    QuadMatrices are 2x2 matrices !
    class FilledRows: public AllFilled {
    public:
      static const int two=2;

      template <class MATRIX>
      FilledRows( const MATRIX & matrix ):AllFilled(two)
      {
	MESSAGE("Quad::FilledRows Ctor");
      }
      
      class FilledCols: public AllFilled {
      public:
	FilledCols( void  ):AllFilled(two)
	{
	  MESSAGE("Quad::FilledRows::FilledCols Ctor");
	}
      };
		
      FilledCols filledColsInRow( int rowIndex ) {
	return FilledCols();
      }
    };

    void iterateOverBlocksMethod(Legolas::MatrixStream & matrixStream) const {

      const QuadMatrix & block=*this;

      const int nr=block.nrows();
      const int nc=block.ncols();
      
      assert(nr!=0);
      assert(nc!=0);
      
      matrixStream.blockInitialize(nr,nc);

      typename Legolas::MatrixShape<level>::Shape brs=block.getRowShape();
      typename Legolas::MatrixShape<level>::Shape bcs=block.getColShape();

      std::vector<int> cornerFlatRowIndexBlock(nr);
      std::vector<int> blockFlatRowSize(nc);
      for (int i=0 ; i < nr ; i++) blockFlatRowSize[i]=Legolas::getSubFlatSize(brs,i);
      cornerFlatRowIndexBlock[0]=matrixStream.cornerFlatRowIndex();
      for (int i=1 ; i < nr ; i++) cornerFlatRowIndexBlock[i]=cornerFlatRowIndexBlock[i-1]+blockFlatRowSize[i-1];
      
      //	for (int i=0 ; i < nr ; i++) INFOS("cornerFlatRowIndexBlock["<<i<<"]="<<cornerFlatRowIndexBlock[i]);
      
      std::vector<int> cornerFlatColIndexBlock(nc);
      std::vector<int> blockFlatColSize(nc);
      for (int j=0 ; j < nc ; j++) blockFlatColSize[j]=Legolas::getSubFlatSize(bcs,j);
      cornerFlatColIndexBlock[0]=matrixStream.cornerFlatColIndex();
      for (int j=1 ; j < nc ; j++) cornerFlatColIndexBlock[j]=cornerFlatColIndexBlock[j-1]+blockFlatColSize[j-1];

      
      for (int i=0 ; i < nr ; i++){     
	matrixStream.rowInitialize(i);
	for (int j=0 ; j < nc ; j++){
	  matrixStream.colInRowInitialize(i,j);
	  matrixStream.createNonEmptyBlock(i,j);
	  
	  MatrixStream * childPtr=matrixStream.child(i,j,
						     cornerFlatRowIndexBlock[i],
						     cornerFlatColIndexBlock[j],
						     blockFlatRowSize[i],
						     blockFlatColSize[j]);
	  if ( (i==0) && (j==0) ) Legolas::iterateOverBlocks(block.M00(),(*childPtr));
	  if ( (i==0) && (j==1) ) Legolas::iterateOverBlocks(block.M01(),(*childPtr));
	  if ( (i==1) && (j==0) ) Legolas::iterateOverBlocks(block.M10(),(*childPtr));
	  if ( (i==1) && (j==1) ) Legolas::iterateOverBlocks(block.M11(),(*childPtr));
	  
	  delete childPtr;
	  
	  matrixStream.colInRowFinalize(i,j);
	}
	matrixStream.rowFinalize(i);
      }
      
      matrixStream.blockFinalize();
      
    }

      
    template <class TRAVERSAL_POLICY>
    struct ElementaryTraversal{
      template <class BLOCK_FILTER,class BLOCK>
      static inline void apply(const BLOCK & block,int i, int j, BLOCK_FILTER & filter){
	INFOS("ICI i="<<i<<" j="<<j);
	typedef typename TRAVERSAL_POLICY:: template Engine<BLOCK_FILTER> ET;
	
	if ( (i==0) && (j==0) ){ ET::applyBlock(block.M00(),filter) ;}
	if ( (i==0) && (j==1) ){ ET::applyBlock(block.M01(),filter) ;}
	if ( (i==1) && (j==0) ){ ET::applyBlock(block.M10(),filter) ;}
	if ( (i==1) && (j==1) ){ ET::applyBlock(block.M11(),filter) ;}
	
      }
    };
    
  private:
    typename MatrixInterface_00::Matrix M00_;
    typename MatrixInterface_01::Matrix M01_;
    typename MatrixInterface_10::Matrix M10_;
    typename MatrixInterface_11::Matrix M11_;
  };

}

#endif
