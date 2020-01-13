/**
 * project  DESCARTES
 *
 * @file     BlockRecursiveTraversal.hxx
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

#include "Legolas/Matrix/MatrixShape.hxx"

#include <string>
#include <fstream>

namespace Legolas{
  
  template <class NON_ZERO_INDEX>
  bool nonZeroElement(int index, NON_ZERO_INDEX & nzi){
    bool result=false;
    int i=nzi.begin();
    while( (i<index) && (i< nzi.end()) ){
      //      INFOS("i="<<i<<" index="<<index<<" nzi.end()="<<nzi.end());
      nzi.next(i);
    }
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

  
  struct BlockRecursiveTraversal{

    template <class BLOCK_FILTER>
    struct Engine{
      
      static inline void applyBlock(const double & element,BLOCK_FILTER & filter){
	filter.storeElement(element);
      }

      static inline void applyBlock(const float & element,BLOCK_FILTER & filter){
	filter.storeElement(element);
      }
      
      static void applyBlock(const AnyMatrix & anyMatrix, BLOCK_FILTER & filter){
	anyMatrix.applyBlockMethod(filter);
      }
      
      template <class BLOCK>
      static inline void applyBlock(const BLOCK & block,BLOCK_FILTER & filter){
	
	const int nr=block.nrows();
	const int nc=block.ncols();

	filter.blockInitialize(nr,nc);

	typename BLOCK::Shape brs=block.getRowShape();
	typename BLOCK::Shape bcs=block.getColShape();
	
	std::vector<int> cornerFlatRowIndexBlock(nr);
	std::vector<int> blockFlatRowSize(nr);
	for (int i=0 ; i < nr ; i++) blockFlatRowSize[i]=Legolas::getSubFlatSize(brs,i);
	cornerFlatRowIndexBlock[0]=filter.cornerFlatRowIndex();
	for (int i=1 ; i < nr ; i++) cornerFlatRowIndexBlock[i]=cornerFlatRowIndexBlock[i-1]+blockFlatRowSize[i-1];
	  
	//	for (int i=0 ; i < nr ; i++) INFOS("cornerFlatRowIndexBlock["<<i<<"]="<<cornerFlatRowIndexBlock[i]);
	
	std::vector<int> cornerFlatColIndexBlock(nc);
	std::vector<int> blockFlatColSize(nc);
	for (int j=0 ; j < nc ; j++) blockFlatColSize[j]=Legolas::getSubFlatSize(bcs,j);
	cornerFlatColIndexBlock[0]=filter.cornerFlatColIndex();
	for (int j=1 ; j < nc ; j++) cornerFlatColIndexBlock[j]=cornerFlatColIndexBlock[j-1]+blockFlatColSize[j-1];

	//	for (int j=1 ; j < nc ; j++) cornerFlatColIndexBlock[j]=cornerFlatColIndexBlock[j-1]+Legolas::getSubFlatSize(bcs,j-1);


	std::vector< std::vector<bool> > emptyness(nr,nc);

	for (int i=0 ; i<nr ; i++){
	  for (int j=0 ; j<nc ; j++){
	    emptyness[i][j]=true;
	  }
	}

	typename BLOCK::FilledRows rows(block);
	for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	  typename BLOCK::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	  
	  for (int j=cols.begin(); j< cols.end() ; cols.next(j)){	    
	    emptyness[i][j]=false;
	  }
	}
	
	for (int i=0 ; i < nr ; i++){     

	  filter.rowInitialize(i);
	
	  for (int j=0 ; j < nc ; j++){

	    filter.colInRowInitialize(i,j);
	    
	    if (!emptyness[i][j]){
	      
	      filter.createNonEmptyBlock(i,j);
	      
	      typedef typename BLOCK::template ElementaryTraversal<BlockRecursiveTraversal> ET;
	      BLOCK_FILTER child=filter.child(i,j,
					      cornerFlatRowIndexBlock[i],
					      cornerFlatColIndexBlock[j],
					      blockFlatRowSize[i],
					      blockFlatColSize[j]);

					      
	      //	      BLOCK_FILTER child=filter.child(i,j,0,0);
	      
	      ET::apply(block,i,j,child);

	    }
	    else{
	      
	      filter.createEmptyBlock(i,j);

	    }

	    filter.colInRowFinalize(i,j);

	  }
	  
	  filter.rowFinalize(i);
	}
      
	filter.blockFinalize();

      }
    };
  };

  static inline void plusEqual(const double & source, double & target){ target+=source;}
  static inline void plusEqual(const float & source, float & target){ target+=source;}

  template <class MATRIX_SOURCE, class MATRIX_TARGET>
  static inline void plusEqual(const MATRIX_SOURCE & source, MATRIX_TARGET & target){
    
    typename MATRIX_SOURCE::FilledRows sourceRows(source);
    
    for (int i=sourceRows.begin(); i<sourceRows.end() ; sourceRows.next(i)){

      typename MATRIX_SOURCE::FilledRows::FilledCols sourceFilledColsInRow=sourceRows.filledColsInRow(i);

      for (int j=sourceFilledColsInRow.begin(); j<sourceFilledColsInRow.end() ; sourceFilledColsInRow.next(j)){

	if (Legolas::zeroElement(i,j,target)){
	  target.sparseGetElement(i,j)=source.sparseGetElement(i,j);
	}
	else{
	  typename MATRIX_SOURCE::ConstGetElement sij=source.sparseGetElement(i,j);
	  typename MATRIX_TARGET::GetElement tij=target.sparseGetElement(i,j);
	  Legolas::plusEqual(sij,tij);
	}
      }
    }
  }

//   template <int level>
//   struct ZeroElement{

//     template <class C>
//     struct Traits{
//       template <class A, class B>
//       C apply(const A & a, const B & b,int i,int j, int k){
    

//   template <class ELEMENT>
//   ELEMENT makeZeroElement( void ){
//     return ELEMENT();
//   }

//   template <>
//   double makeZeroElementWithProperShape<double>( void ){ return 0.0;}

//   template <>
//   float makeZeroElement<float>( void ){ return 0.0;}
  



  static inline void plusEqualProduct(const double & a, const double & b, double & c){ c+=a*b;}
  static inline void plusEqualProduct(const float & a, const float & b, float & c){ c+=a*b;}

  //C+=A*B
  template <class MATRIX_A, class MATRIX_B, class MATRIX_C>
  static inline void plusEqualProduct(const MATRIX_A & A, const MATRIX_B & B, MATRIX_C & C){
    
    typedef typename MATRIX_A::RealType RealType;
    typedef typename MATRIX_C::MatrixElementInterface::Copy CElement;
    
    Legolas::MatrixShape<MATRIX_C::level> matrixShapeC(A.getRowShape(),B.getColShape());
    
    C.reShape(matrixShapeC);
    
    for (int i=0 ; i< A.nrows() ; i++){
      for (int j=0 ; j< B.ncols() ; j++){
	for (int k=0 ; k< A.ncols() ; k++){
	  if (!(zeroElement(i,k,A))&&(!zeroElement(k,j,B))){
	    typename MATRIX_C::GetElement cij=C.sparseGetElement(i,j);
	    Legolas::plusEqualProduct(A.sparseGetElement(i,k),B.sparseGetElement(k,j),cij);
	  }
	}
      }
    }
  }




  
  
}//end namespace legolas

#endif
  



















