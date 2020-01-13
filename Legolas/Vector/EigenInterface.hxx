#ifndef __EIGENINTERFACE_HXX__
#define __EIGENINTERFACE_HXX__

#ifdef LEGOLAS_USE_EIGEN

#include <Eigen/Core>

namespace Legolas{
  
  template <class REAL_TYPE>
  class EigenInterface{
  public:
    typedef REAL_TYPE RealType;
    //    typedef Eigen::Matrix<RealType,Eigen::Dynamic,1>     EigenVector;
    typedef Eigen::Array<RealType,Eigen::Dynamic,1>     EigenVector;
    typedef Eigen::Matrix<RealType,Eigen::Dynamic,1>    EigenMatrix;
    //    typedef Eigen::Map<EigenVector>        EigenVectorView;
    //    typedef Eigen::Map<EigenVector,Eigen::Aligned>        EigenVectorView;
    //    typedef Eigen::Map<EigenVector>        EigenVectorView;
    typedef Eigen::Map<EigenVector,Eigen::Aligned>        EigenVectorView;
    typedef Eigen::Map<EigenMatrix,Eigen::Aligned>        EigenMatrixView;

  };
  
}

#endif


#endif






