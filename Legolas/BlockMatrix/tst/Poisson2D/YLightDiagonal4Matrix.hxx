#ifndef __LEGOLAS_YLIGHTDIAGONAL4MATRIX_HXX__
#define __LEGOLAS_YLIGHTDIAGONAL4MATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMatrixOptions.hxx"

class YLightDiagonal4ScalarContainer{
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
    
  };
};

struct YLightDiagonal4Options: public Legolas::DiagonalScalarMatrixOptions{
  typedef YLightDiagonal4ScalarContainer DiagonalContainer;
};

template <class SCALAR>
class YLightDiagonal4Matrix : public Legolas::DiagonalScalarMatrix<SCALAR,YLightDiagonal4Options> {
  typedef SCALAR RealType;
  typedef Legolas::DiagonalScalarMatrix<SCALAR,YLightDiagonal4Options> Base;
  typedef typename Base::Container Container;
public:
  
  std::string name( void ) const{ return "YLightDiagonal4Matrix";}
  
  YLightDiagonal4Matrix(const Legolas::MatrixShape<1> & ms):Base(Container(ms)){}
  
  
};
  
#endif
    
