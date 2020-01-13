#ifndef __UNIFORMMESH_HXX__
#define __UNIFORMMESH_HXX__

#include "Legolas/Vector/Vector.hxx"

template <class REAL_TYPE>
struct UniformMesh{
  
  typedef REAL_TYPE Real;
  typedef Legolas::Vector<Real> Vector1D;
  
  static inline Vector1D build(int meshSize){
    Vector1D result(meshSize);
    Real stepSize=1.0/Real(meshSize-1);
    for (int i = 0 ; i < meshSize ; i++) result[i]=Real(i)*stepSize;
    return result;
  }
};
    
#endif

