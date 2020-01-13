#ifndef __POLYMORPHICMATRIXINTERFACE__
#define __POLYMORPHICMATRIXINTERFACE__

#include "PolymorphicMatrix.hxx"
#include "Legolas/Algorithm/PolymorphicSolver.hxx"

namespace Legolas{
  
  template <int LEVEL, class REAL_TYPE>
  class PolymorphicMatrixInterface{
  public:

    typedef PolymorphicMatrix Matrix;

    struct DataDriver{

      
      typedef  PolymorphicMatrix Data;
		
      static const int level=LEVEL;
      typedef REAL_TYPE   RealType;
		
      static inline void copy(const Data & inputMatrix, Data & matrix){
	matrix=inputMatrix;
      }
    };
      
      
    typedef Matrix   Copy;
    typedef Matrix & View;
    typedef const Matrix & ConstView;

    typedef Legolas::PolymorphicSolver Solver;

 
  };

}

#endif
