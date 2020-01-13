#ifndef __LEGOLAS_MYTRIDIAGONALMATRIX_HXX__
#define __LEGOLAS_MYTRIDIAGONALMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMultOperator.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarMatrix.hxx"

struct MyTriDiagonalOptions: public Legolas::TriDiagonalScalarMatrixOptions{
  //  typedef Legolas::BandedScalarGaussSeidelSolver Solver;
  //  typedef Legolas::BandedScalarMultOperator MultOperator;
};



template <class SCALAR>
class MyTriDiagonalMatrix : public Legolas::TriDiagonalScalarMatrix<SCALAR,MyTriDiagonalOptions> {
  typedef SCALAR RealType;
  typedef Legolas::TriDiagonalScalarMatrix<SCALAR,MyTriDiagonalOptions> Base;
  typedef Legolas::MultiVector<RealType,1> V1D;
public:
  
  std::string name( void ) const { return "MyTriDiagonalMatrix" ;}

  MyTriDiagonalMatrix(const Legolas::MatrixShape<1> & ms):Base(ms){
    for (int i=0 ; i< this->nrows() ; i++){
      for (int j=std::max(0,i-1) ; j< std::min(this->nrows(),i+2) ; j++){
	if(i==j) (*this).diagonalGetElement(i)=4.0;
	if(j==i-1) (*this).lowerDiagonalGetElement(i)=-1.0;
	if(j==i+1) (*this).upperDiagonalGetElement(i)=-1.0;
      }
    }
  }
  
  
};
  
#endif
    
