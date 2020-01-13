#ifndef __LEGOLAS_TRIDIAGONALSCALARCONTAINER_HXX__
#define __LEGOLAS_TRIDIAGONALSCALARCONTAINER_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  

  class TriDiagonalScalarContainer{
  public:
    template <class SCALAR>
    class Engine: public Matrix{
    public:
      typedef SCALAR RealType;
      typedef Engine<SCALAR> Container;
    private:
      typedef Legolas::MultiVector<SCALAR,2>  TriDiagonalData;
      
      TriDiagonalData tridiagonalData_;

      typedef typename Legolas::MultiVector<SCALAR,2>::Shape  Shape2D;
      
    public:
      
      inline Engine():Matrix(),tridiagonalData_(){}
      inline Engine(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape),
								   tridiagonalData_(Shape2D(3,virtualMatrixShape.nrows())){}
      inline Engine(const Container & container):Matrix(container),
						 tridiagonalData_(container.tridiagonalData_){}

      inline const SCALAR & lowerDiagonalGetElement(int i) const{ return tridiagonalData_[0][i];}
      inline SCALAR & lowerDiagonalGetElement(int i){ return tridiagonalData_[0][i];}
      
      inline const SCALAR & diagonalGetElement(int i) const{ return tridiagonalData_[1][i];}
      inline SCALAR & diagonalGetElement(int i){ return tridiagonalData_[1][i];}

      inline const SCALAR & upperDiagonalGetElement(int i) const{ return tridiagonalData_[2][i];}
      inline SCALAR & upperDiagonalGetElement(int i){ return tridiagonalData_[2][i];}


      
    };

  };

}

#endif
    
    
    
    
