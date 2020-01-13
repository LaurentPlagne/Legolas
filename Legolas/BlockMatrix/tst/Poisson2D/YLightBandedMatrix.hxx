#ifndef __LEGOLAS_YLIGHTBANDEDMATRIX_HXX__
#define __LEGOLAS_YLIGHTBANDEDMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMatrix.hxx"

#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMatrixOptions.hxx"

class YLightBandedScalarContainer{
public:
  template <class SCALAR>
  class Engine: public Legolas::Matrix{
  public:
    typedef SCALAR RealType;
    typedef Engine<SCALAR> Container;

    SCALAR diagonalElement_;
    SCALAR upperDiagonalElement_;
    int lsup_;
    
  public:
    
    //    inline Engine(const Legolas::VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape){}

    virtual std::string name( void ) const{
      return "toto";
    }
    
    virtual int getPrecisionInBytePerRealElement( void ) const{
      return sizeof(RealType);
    }

    virtual void collectStatistics(Legolas::Profile & profile) const{}

    virtual void iterateOverElements(Legolas::MatrixStream & matrixStream) const{}


    inline Engine(const Container & container):Matrix(container),
					       diagonalElement_(container.diagonalElement_),
					       upperDiagonalElement_(container.upperDiagonalElement_),
					       lsup_(container.lsup_){}
    
    
    inline Engine(const Legolas::VirtualMatrixShape & virtualMatrixShape,
		  const bool & isDiagonal ):Matrix(virtualMatrixShape),
					    diagonalElement_(0.0),
					    upperDiagonalElement_(0.0),
					    lsup_(0)
    {
      if (isDiagonal){
	diagonalElement_=-4.0;	
	upperDiagonalElement_=1.0; 
	lsup_=1;
      }
      else{
	diagonalElement_=1.0;
      }
    }

    inline int lsup( void ) const { return lsup_;}
    inline int linf( void ) const { return lsup_;}
    
    inline SCALAR bandedGetElement(int i, int j) const{ 
      if (i!=j){
	return upperDiagonalElement_;
      }
      else{
	return diagonalElement_;
      }
    }
      
  };
};

struct YLightBandedOptions: public Legolas::BandedScalarMatrixOptions{
  typedef Legolas::BandedScalarGaussSeidelSolver Solver;
  typedef YLightBandedScalarContainer BandedContainer;
};

template <class SCALAR>
class YLightBandedMatrix : public Legolas::BandedScalarMatrix<SCALAR,YLightBandedOptions> {
  typedef SCALAR RealType;
  typedef Legolas::BandedScalarMatrix<SCALAR,YLightBandedOptions> Base;
  typedef typename Base::Container Container;
public:
  
  std::string name( void ) const{ return "YLightBandedMatrix";}
  
  YLightBandedMatrix(const Legolas::MatrixShape<1> & ms, const bool & isDiagonal):Base(Container(ms,isDiagonal)){}
  
  
};
  
#endif
    
