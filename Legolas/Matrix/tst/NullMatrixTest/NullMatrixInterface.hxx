#ifndef __LEGOLAS_NULLMATRIXINTERFACE_HXX__
#define __LEGOLAS_NULLMATRIXINTERFACE_HXX__

#include "NullMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"

namespace Legolas{

  template <class REAL_TYPE, int LEVEL>
  class NullMatrixInterface: 
    public Legolas::GenericMatrixInterface< 
    Legolas::NullMatrixDefinition<REAL_TYPE,LEVEL>,
    Legolas::InputMatrixOptions<>,
    Legolas::NullMatrixInterface<REAL_TYPE,LEVEL-1> >
  {
  };


  template <class REAL_TYPE>
  class NullMatrixInterface<REAL_TYPE,1>: 
    public Legolas::GenericMatrixInterface<
    Legolas::NullMatrixDefinition<REAL_TYPE,1>,
    Legolas::InputMatrixOptions<>,
    Legolas::RealElementInterface<REAL_TYPE> > 
  {
  };
  
}
    
#endif

  
