#ifndef __LEGOLAS_YLIGHTDIAGONALMATRIX_HXX__
#define __LEGOLAS_YLIGHTDIAGONALMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrixOptions.hxx"

class YLightDiagonalScalarContainer{
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

    
    inline SCALAR diagonalGetElement(int i) const{ return 1.0 ;}
    
  };
};

struct YLightDiagonalOptions: public Legolas::DiagonalScalarMatrixOptions{
  typedef YLightDiagonalScalarContainer DiagonalContainer;
};

template <class SCALAR>
class YLightDiagonalMatrix : public Legolas::DiagonalScalarMatrix<SCALAR,YLightDiagonalOptions> {
  typedef SCALAR RealType;
  typedef Legolas::DiagonalScalarMatrix<SCALAR,YLightDiagonalOptions> Base;
  typedef typename Base::Container Container;
public:
  
  std::string name( void ) const{ return "YLightDiagonalMatrix";}
  
  YLightDiagonalMatrix(const Legolas::MatrixShape<1> & ms):Base(Container(ms)){}
  
  
};
  
#endif
    
