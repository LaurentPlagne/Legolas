/**
 * project  DESCARTES
 *
 * @file     MatrixVectorOperations.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_BLOCKRECURSIVETRAVERSAL_HXX__
#define __LEGOLAS_BLOCKRECURSIVETRAVERSAL_HXX__

#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/MatrixVectorExpression.hxx"

#include <string>
#include <fstream>

namespace Legolas{
  
  template <class NON_ZERO_INDEX>
  bool nonZeroElement(int index, const NON_ZERO_INDEX & nzi){
    bool result=false;
    int i=nzi.begin();
    while( (i<index) && (i< nzi.end()) ) nzi.next(i);
    
    if (i==index && (i!=nzi.end()) ) result=true;
    
    return result;
  }

  template <class BLOCK>
  bool zeroElement(int rowIndex, int colIndex, const BLOCK & block){

    bool result=true;
    typename BLOCK::FilledRows rows(block);
    bool emptyRow=!nonZeroElement(rowIndex,rows);

    if (!emptyRow){
      typename BLOCK::FilledRows::FilledCols cols=rows.filledColsInRow(rowIndex);
      result=!nonZeroElement(colIndex,cols);
    }
    return result;
  }

  
  template <class BLOCK_FILTER>
  struct BlockRecursiveTraversal{

    template <class BLOCK>
    static inline void apply(const BLOCK & block,BLOCK_FILTER & filter){
      const double zero=0.0;
	
      const int nr=block.nrows();
      const int nc=block.ncols();
      
      filter.blockInitialize(nr,nc);
      
      for (int i=0 ; i < nr ; i++){     
	
	filter.rowInitialize(i,nr,nc);
	
	for (int j=0 ; j < nc ; j++){
	  
	  filter.colInRowInitialize(i,j,nr,nc);
	  
	  if (!zeroElement(i,j,block)){
	     BlockRecursiveTraversal<LatexFilter>::apply(block.sparseGetElement(i,j),filter);
	  }
	  else{
	     BlockRecursiveTraversal<LatexFilter>::apply(zero,filter);
	  }
	  
	  filter.colInRowFinalize(i,j,nr,nc);
	}
	
	filter.rowFinalize(i,nr,nc);
      }
      
      filter.blockFinalize(nr,nc);
    }
    
    static inline void apply(const double & element,BLOCK_FILTER & filter){
      filter.storeElement(element);
    }

  };
}//end namespace legolas

#endif
  



















