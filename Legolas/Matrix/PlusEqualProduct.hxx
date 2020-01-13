#ifndef __LEGOLAS_PLUSEQUALPRODUCT_HXX__
#define __LEGOLAS_PLUSEQUALPRODUCT_HXX__

// #include "Legolas/Vector/BaseVector.hxx"
// #include "Legolas/Vector/BaseMatrix.hxx"
// #include "Legolas/Vector/Vector.hxx"
// #include "Legolas/Vector/Expression/MatrixVectorExpression.hxx"

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
}

#endif
