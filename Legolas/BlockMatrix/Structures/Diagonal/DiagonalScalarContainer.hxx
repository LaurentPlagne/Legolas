#ifndef __LEGOLAS_DIAGONALSCALARCONTAINER_HXX__
#define __LEGOLAS_DIAGONALSCALARCONTAINER_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  

  class DiagonalScalarContainer{
  public:
    template <class SCALAR>
    class Engine: public Matrix{
    public:
      typedef SCALAR RealType;
      typedef Engine<SCALAR> Container;
    private:
      typedef Legolas::MultiVector<SCALAR,1>  DiagonalData;
      
      DiagonalData diagonalData_;
      
    public:
      
      inline Engine():Matrix(),diagonalData_(){}
      inline Engine(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape),
								   diagonalData_(virtualMatrixShape.nrows()){}
      inline Engine(const Container & container):Matrix(container),
						 diagonalData_(container.diagonalData_){}
      
      inline const SCALAR & diagonalGetElement(int i) const{ return diagonalData_[i];}
      inline SCALAR & diagonalGetElement(int i){ return diagonalData_[i];}
      
    };

  };

}

#endif
    
    
    
    
