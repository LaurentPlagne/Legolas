#ifndef __LEGOLAS_LIGHTDIAGONALSCALARCONTAINER_HXX__
#define __LEGOLAS_LIGHTDIAGONALSCALARCONTAINER_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  

  class LightDiagonalScalarContainer{
  public:
    template <class SCALAR>
    class Engine: public Matrix{
    public:
      typedef SCALAR RealType;
    private:
      typedef Legolas::MultiVector<SCALAR,1>  LightDiagonalData;
      
      LightDiagonalData lightdiagonalData_;
      
    public:
      
      inline Engine():Matrix(),lightdiagonalData_(){}
      inline Engine(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape),
								   lightdiagonalData_(virtualMatrixShape.nrows()){}
      
      inline const SCALAR & lightdiagonalGetElement(int i) const{ return lightdiagonalData_[i];}
      inline SCALAR & lightdiagonalGetElement(int i){ return lightdiagonalData_[i];}
      
    };

  };

}

#endif
    
    
    
    
