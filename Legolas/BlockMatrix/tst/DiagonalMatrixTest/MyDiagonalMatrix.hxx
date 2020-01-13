#ifndef __LEGOLAS_MYDIAGONALMATRIX_HXX__
#define __LEGOLAS_MYDIAGONALMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrix.hxx"

#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrixOptions.hxx"

struct MyDiagonalOptions: public Legolas::DiagonalScalarMatrixOptions{
  typedef Legolas::BandedScalarGaussSeidelSolver Solver;
};

template <class SCALAR>
class MyDiagonalMatrix : public Legolas::DiagonalScalarMatrix<SCALAR,MyDiagonalOptions> {
  typedef SCALAR RealType;
  typedef Legolas::DiagonalScalarMatrix<SCALAR,MyDiagonalOptions> Base;
  typedef Legolas::MultiVector<RealType,1> V1D;
  typedef typename Base::Container Container;
public:
  
  std::string name( void ) const{ return "MyDiagonalMatrix";}

  MyDiagonalMatrix(const Legolas::MatrixShape<1> & ms):Base(ms){
    for (int i=0 ; i< this->nrows() ; i++){
      (*this).diagonalGetElement(i)=4.0;
    }
  }

  static inline Container buildContainer(const Legolas::MatrixShape<1> & ms, double value){
    INFOS("ICI");
    Container container(ms);
    for (int i=0 ; i< container.nrows() ; i++){
      container.diagonalGetElement(i)=value;
    }
    return container;
  }
  
  
  MyDiagonalMatrix(const Legolas::MatrixShape<1> & ms, double value):Base(buildContainer(ms,value)){}
  
  
};
  
#endif
    
