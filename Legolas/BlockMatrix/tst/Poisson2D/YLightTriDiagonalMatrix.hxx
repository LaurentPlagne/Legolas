#ifndef __LEGOLAS_YLIGHTTRIDIAGONALMATRIX_HXX__
#define __LEGOLAS_YLIGHTTRIDIAGONALMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarMatrixOptions.hxx"

class YLightTriDiagonalScalarContainer{
public:
  template <class SCALAR>
  class Engine: public Legolas::Matrix{
  public:
    typedef SCALAR RealType;
    typedef Engine<SCALAR> Container;

  public:

    virtual std::string name( void ) const{
      return "toto";
    }
    
    virtual int getPrecisionInBytePerRealElement( void ) const{
      return sizeof(RealType);
    }

    virtual void collectStatistics(Legolas::Profile & profile) const{}
    virtual void iterateOverElements(Legolas::MatrixStream & matrixStream) const{}

    
    inline Engine(const Container & container):Matrix(container){}
    inline Engine(const Legolas::VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape){}

    inline SCALAR diagonalGetElement(int i) const{ return -4.0 ;}
    inline SCALAR lowerDiagonalGetElement(int i) const{ return 1.0 ;}
    inline SCALAR upperDiagonalGetElement(int i) const{ return 1.0 ;}
    
  };
};

struct YLightTriDiagonalOptions: public Legolas::TriDiagonalScalarMatrixOptions{
  typedef YLightTriDiagonalScalarContainer TriDiagonalContainer;
};

template <class SCALAR>
class YLightTriDiagonalMatrix : public Legolas::TriDiagonalScalarMatrix<SCALAR,YLightTriDiagonalOptions> {
  typedef SCALAR RealType;
  typedef Legolas::TriDiagonalScalarMatrix<SCALAR,YLightTriDiagonalOptions> Base;
  typedef typename Base::Container Container;
public:
  
  std::string name( void ) const{ return "YLightTriDiagonalMatrix";}
  
  YLightTriDiagonalMatrix(const Legolas::MatrixShape<1> & ms):Base(Container(ms)){
    this->maxIteration()=1000;
    this->fixedIterationNumber()=false;
    this->epsilon()=1.e-6;
  }
  
  
};
  
#endif
    
