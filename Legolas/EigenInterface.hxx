#pragma once
#include <Eigen/Core>

namespace Legolas{

// template <class BLOCK>
// struct DynamicArrayTraits{
// };

// template <class SCALAR_TYPE, int PACK_SIZE>
// struct DynamicArrayTraits<Eigen::Array<


// template <class SCALAR_TYPE, int PACK_SIZE>


template <class BLOCK, int modulo>
struct EigenBlockMapTraits{
  typedef Eigen::Map<BLOCK> Map;
  typedef typename BLOCK::Scalar Scalar;
};


#if defined(__AVX512F__)
static const int DEFAULT_ALIGN_BYTES = 64;
#elif defined(__AVX__) || defined(__AVX2__)
static const int DEFAULT_ALIGN_BYTES = 32;
#else
static const int DEFAULT_ALIGN_BYTES = 16;
#endif

template <class BLOCK>
struct EigenBlockMapTraits<BLOCK,0>{
  typedef Eigen::Map<BLOCK,Eigen::Unaligned> Map;
};

// 16 pour NEON/SSE et 32 pour AVX
template <class BLOCK, int BYTE=DEFAULT_ALIGN_BYTES>  
struct EigenBlockMap{
  static const int modulo=sizeof(BLOCK)%BYTE;
  typedef typename EigenBlockMapTraits<BLOCK,modulo>::Map Map;

  typedef typename BLOCK::Scalar SCALAR_TYPE;
  typedef Eigen::Array<SCALAR_TYPE,Eigen::Dynamic,1> EigenArray;
  typedef typename EigenBlockMapTraits<EigenArray,modulo>::Map EigenArrayView;
  typedef typename EigenBlockMapTraits<const EigenArray,modulo>::Map ConstEigenArrayView;

};


// template <class SCALAR_TYPE>
// struct EigenInterface{
// };

template <class SCALAR_TYPE, int PACK_SIZE>
struct EigenInterface{

  static const int packSize_=PACK_SIZE;
  static inline int shift(int i){return i*packSize_;}

  typedef Eigen::Matrix<SCALAR_TYPE,packSize_, 1> BlockVector;
  typedef typename EigenBlockMap<BlockVector>::Map BlockVectorView;
  typedef typename EigenBlockMap<const BlockVector>::Map ConstBlockVectorView;

  typedef Eigen::Array<SCALAR_TYPE,packSize_, 1> BlockArray;
  typedef typename EigenBlockMap<BlockArray>::Map BlockArrayView;
  typedef typename EigenBlockMap<const BlockArray>::Map ConstBlockArrayView;

  typedef Eigen::Matrix<SCALAR_TYPE,packSize_,packSize_> BlockMatrix;
  typedef typename EigenBlockMap<BlockMatrix>::Map BlockMatrixView;
  typedef typename EigenBlockMap<const BlockMatrix>::Map ConstBlockMatrixView;

  typedef Eigen::Array<SCALAR_TYPE,Eigen::Dynamic,1>     EigenVector;//deprecated
  typedef Eigen::Array<SCALAR_TYPE,Eigen::Dynamic,1>     EigenArray;
  typedef Eigen::Matrix<SCALAR_TYPE,Eigen::Dynamic,1>    EigenMatrix;

  typedef typename EigenBlockMap<BlockArray>::EigenArrayView EigenArrayView;
  typedef typename EigenBlockMap<BlockArray>::ConstEigenArrayView ConstEigenArrayView;

  
};

  // template <>
  // struct EigenInterface<double>{
  //   static const int packSize_=2;

  //   static inline int shift(int i){return i*packSize_;}

  //   typedef Eigen::Matrix<double,packSize_, 1> BlockVector;
  //   typedef typename EigenBlockMap<BlockVector>::Map BlockVectorView;
  //   typedef typename EigenBlockMap<const BlockVector>::Map ConstBlockVectorView;

  //   typedef Eigen::Array<double,packSize_, 1> BlockArray;
  //   typedef typename EigenBlockMap<BlockArray>::Map BlockArrayView;
  //   typedef typename EigenBlockMap<const BlockArray>::Map ConstBlockArrayView;

  //   typedef Eigen::Matrix<double,packSize_,packSize_> BlockMatrix;
  //   typedef typename EigenBlockMap<BlockMatrix>::Map BlockMatrixView;
  //   typedef typename EigenBlockMap<const BlockMatrix>::Map ConstBlockMatrixView;

  //   typedef Eigen::Array<double,Eigen::Dynamic,1>     EigenVector;
  //   typedef Eigen::Matrix<double,Eigen::Dynamic,1>    EigenMatrix;


  // };
}
