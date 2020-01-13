#ifndef __LEGOLAS_MYLIGHTDIAGONALMATRIX_HXX__
#define __LEGOLAS_MYLIGHTDIAGONALMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrix.hxx"

#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrixOptions.hxx"

class MyLightDiagonalScalarContainer{
public:
  template <class SCALAR>
  class Engine: public Legolas::Matrix{
  public:
    typedef SCALAR RealType;
    typedef Engine<SCALAR> Container;

    SCALAR factor_;

  public:
    
    inline Engine(const Legolas::VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape){}
    inline Engine(const Container & container, const SCALAR & factor):Matrix(container),factor_(factor){}
    
    inline SCALAR diagonalGetElement(int i) const{ return factor_*SCALAR(i+1);}
    
  };
};

struct MyLightDiagonalOptions: public Legolas::DiagonalScalarMatrixOptions{
  typedef Legolas::BandedScalarGaussSeidelSolver Solver;
  typedef MyLightDiagonalScalarContainer DiagonalContainer;
};

template <class SCALAR>
class MyLightDiagonalMatrix : public Legolas::DiagonalScalarMatrix<SCALAR,MyLightDiagonalOptions> {
  typedef SCALAR RealType;
  typedef Legolas::DiagonalScalarMatrix<SCALAR,MyLightDiagonalOptions> Base;
  typedef typename Base::Container Container;
public:
  
  std::string name( void ) const{ return "MyLightDiagonalMatrix";}
  
  MyLightDiagonalMatrix(const Legolas::MatrixShape<1> & ms, const SCALAR & factor=1):Base(Container(ms,factor)){}
  
  
};
  
#endif
    
