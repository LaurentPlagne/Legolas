#pragma once
#include "RealType.hxx"
#include "SSEDataTraits.hxx"

template <class REAL_TYPE>
struct SSETraits{
  static const int packSize=1;
};

template <>
struct SSETraits<double>{
  static const int packSize=2;
};

template <>
struct SSEDataTraits<float>{
  static const int packSize=4;
};



template <class SCALAR>
class EigenWrapper{
  
  typedef SCALAR Scalar;
  
  static const int blockSize=SSETraits<Scalar>::packSize=;
  
  typedef Legolas::MultiVector<Scalar,1> V1D;
  
  typedef Eigen::Array<Scalar, blockSize, 1> BlockArray;
  typedef Eigen::Map<BlockArray,Eigen::Aligned>  BlockArrayView;
  typedef Eigen::Map<const BlockArray,Eigen::Aligned>  ConstBlockArrayView;

  V1D & v1D_;
  
public:

  EigenWrapper( V1D & v1D):v1D_(v1D){}

  inline BlockArrayView operator[] (int i){ return BlockArrayView(&v1D_[i]) };
  inline ConstBlockArrayView operator[] (int i) const { return BlockArrayView(&v1D_[i]) };
};

template <class SCALAR>
class ConstEigenWrapper{
  
  typedef SCALAR Scalar;
  
  static const int blockSize=SSETraits<Scalar>::packSize=;
  
  typedef Legolas::MultiVector<Scalar,1> V1D;
  
  typedef Eigen::Array<Scalar, blockSize, 1> BlockArray;
  typedef Eigen::Map<BlockArray,Eigen::Aligned>  BlockArrayView;
  typedef Eigen::Map<const BlockArray,Eigen::Aligned>  ConstBlockArrayView;

  const V1D & v1D_;
  
public:

  EigenWrapper(const V1D & v1D):v1D_(v1D){}

  inline ConstBlockArrayView operator[] (int i) const { return BlockArrayView(&v1D_[i]) };
};

    
  
  
  
