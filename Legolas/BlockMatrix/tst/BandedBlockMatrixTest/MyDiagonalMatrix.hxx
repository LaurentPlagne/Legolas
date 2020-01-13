#ifndef __LEGOLAS_MYDIAGONALMATRIX_HXX__
#define __LEGOLAS_MYDIAGONALMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrix.hxx"

template <class REAL_TYPE>
class MyDiagonalMatrix : public Legolas::DiagonalScalarMatrix<REAL_TYPE> {
  typedef REAL_TYPE RealType;
  typedef Legolas::DiagonalScalarMatrix<REAL_TYPE> Base;
  typedef Legolas::MultiVector<RealType,1> V1D;
public:

  std::string name( void ) const { return "MyDiagonalMatrix" ;}
  
  MyDiagonalMatrix(const Legolas::MatrixShape<1> & ms, double value):Base(ms){
    for (int i=0 ; i< this->nrows() ; i++){
      (*this).diagonalGetElement(i)=value;
    }
  }
  

};
  
#endif
    
