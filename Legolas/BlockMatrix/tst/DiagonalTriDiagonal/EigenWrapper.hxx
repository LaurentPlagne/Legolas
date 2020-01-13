#pragma once
#include "RealType.hxx"

template <class REAL_TYPE>
struct SSETraits{
  static const int packSize=1;
};

template <>
struct SSETraits<double>{
  static const int packSize=2;
};

template <>
struct SSETraits<float>{
  static const int packSize=4;
};



template <class SCALAR>
class EigenWrapper{
public:  
  typedef SCALAR Scalar;
  
  static const int blockSize=SSETraits<Scalar>::packSize;
  
  typedef Legolas::MultiVector<Scalar,1> V1D;
  
  typedef Eigen::Array<Scalar, blockSize, 1> BlockArray;

  typedef Eigen::Map<BlockArray,Eigen::Aligned>  BlockArrayView;
  typedef Eigen::Map<const BlockArray,Eigen::Aligned>  ConstBlockArrayView;



  V1D & v1D_;
  
public:

  EigenWrapper( V1D & v1D):v1D_(v1D){}

  
  inline BlockArrayView operator[] (int i){ return BlockArrayView(&v1D_[i*blockSize]) ;}
  inline ConstBlockArrayView operator[] (int i) const { return ConstBlockArrayView(&v1D_[i*blockSize]) ;}
};

template <class SCALAR>
class ConstEigenWrapper{
public:  
  typedef SCALAR Scalar;
  
  static const int blockSize=SSETraits<Scalar>::packSize;
  
  typedef Legolas::MultiVector<Scalar,1> V1D;
  
  typedef Eigen::Array<Scalar, blockSize, 1> BlockArray;
  typedef Eigen::Map<BlockArray,Eigen::Aligned>  BlockArrayView;
  typedef Eigen::Map<const BlockArray,Eigen::Aligned>  ConstBlockArrayView;

  const V1D & v1D_;
  


  ConstEigenWrapper(const V1D & v1D):v1D_(v1D){}

  inline ConstBlockArrayView operator[] (int i) const { return ConstBlockArrayView(&v1D_[i*blockSize]) ;}
};

    
  
  
  
