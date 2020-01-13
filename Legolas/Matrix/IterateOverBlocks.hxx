#ifndef __LEGOLAS_ITERATEOVERBLOCKS_HXX__
#define __LEGOLAS_ITERATEOVERBLOCKS_HXX__

#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Matrix/MatrixStream.hxx"

namespace Legolas{

  inline void iterateOverBlocks(const AnyMatrix & anyMatrix,
			 Legolas::MatrixStream & aStream){
    anyMatrix.iterateOverBlocksMethod(aStream);
  }

  inline void iterateOverBlocks(const double & aDouble,
			 Legolas::MatrixStream & aStream){
    aStream.dumpElement(0,0,aDouble);
  }

  inline void iterateOverBlocks(const float & aFloat,
			 Legolas::MatrixStream & aStream){
    aStream.dumpElement(0,0,aFloat);
  }

  struct IterateOverBlocks{
    
    template <class BLOCK>
    static inline void apply(const BLOCK & block, Legolas::MatrixStream & matrixStream){
      
      const int nr=block.nrows();
      const int nc=block.ncols();

      assert(nr!=0);
      assert(nc!=0);

      matrixStream.blockInitialize(nr,nc);

      const int level=BLOCK::level;
      typedef typename BLOCK::FilledRows FilledRows;      
      
      typename Legolas::MatrixShape<level>::Shape brs=block.getRowShape();
      typename Legolas::MatrixShape<level>::Shape bcs=block.getColShape();
      //      Shape bcs=block.getColShape();
      

      std::vector<int> cornerFlatRowIndexBlock(nr);
      std::vector<int> blockFlatRowSize(nr);
      //      for (int i=0 ; i < nr ; i++) blockFlatRowSize[i]=Legolas::getSubFlatSize(brs,i);
      for (int i=0 ; i < nr ; i++) blockFlatRowSize[i]=brs.subFlatSize(i);
      cornerFlatRowIndexBlock[0]=matrixStream.cornerFlatRowIndex();
      for (int i=1 ; i < nr ; i++) cornerFlatRowIndexBlock[i]=cornerFlatRowIndexBlock[i-1]+blockFlatRowSize[i-1];
      
      //	for (int i=0 ; i < nr ; i++) INFOS("cornerFlatRowIndexBlock["<<i<<"]="<<cornerFlatRowIndexBlock[i]);
      
      std::vector<int> cornerFlatColIndexBlock(nc);
      std::vector<int> blockFlatColSize(nc);
      //      for (int j=0 ; j < nc ; j++) blockFlatColSize[j]=Legolas::getSubFlatSize(bcs,j);
      for (int j=0 ; j < nc ; j++) blockFlatColSize[j]=bcs.subFlatSize(j);
      cornerFlatColIndexBlock[0]=matrixStream.cornerFlatColIndex();
      for (int j=1 ; j < nc ; j++) cornerFlatColIndexBlock[j]=cornerFlatColIndexBlock[j-1]+blockFlatColSize[j-1];
      
      //	for (int j=1 ; j < nc ; j++) cornerFlatColIndexBlock[j]=cornerFlatColIndexBlock[j-1]+Legolas::getSubFlatSize(bcs,j-1);

      std::vector<bool> emptycol(nc,true);
      std::vector< std::vector<bool> > emptyness(nr,emptycol);
      
      
      FilledRows rows(block);
      for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	
	for (int j=cols.begin(); j< cols.end() ; cols.next(j)){	    
	  emptyness[i][j]=false;
	}
      }
      
      for (int i=0 ; i < nr ; i++){     
	matrixStream.rowInitialize(i);
	for (int j=0 ; j < nc ; j++){
	  matrixStream.colInRowInitialize(i,j);
	  if (!emptyness[i][j]){
	    matrixStream.createNonEmptyBlock(i,j);

	    MatrixStream * childPtr=matrixStream.child(i,j,
						       cornerFlatRowIndexBlock[i],
						       cornerFlatColIndexBlock[j],
						       blockFlatRowSize[i],
						       blockFlatColSize[j]);

	    

	    Legolas::iterateOverBlocks(block.sparseGetElement(i,j),(*childPtr));

	    delete childPtr;
	    
	  }
	  else{
	    matrixStream.createEmptyBlock(i,j);
	  }
	  matrixStream.colInRowFinalize(i,j);
	}
	matrixStream.rowFinalize(i);
      }
      matrixStream.blockFinalize();
    }

  };

  
}

#endif
  
