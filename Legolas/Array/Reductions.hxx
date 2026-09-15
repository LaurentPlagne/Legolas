#pragma once

#include "Legolas/Array/NativeSimd.hxx"

#include <cstddef>
#include <type_traits>
#include <utility>

#ifdef LEGOLAS_HAS_VULKAN
// Automatic GPU dispatch: when the Vulkan backend is enabled (link
// Legolas::Vulkan), the public squaredNorm/dot route eligible flat float
// arrays through the device. LEGOLAS_DISABLE_VULKAN=1 forces the CPU path and
// LEGOLAS_VULKAN_MIN_ELEMENTS tunes the minimum size (default 2^20 elements).
#  include "Legolas/Vulkan/Backend.hxx"
#  include "Legolas/Vulkan/Reductions.hxx"
#endif

namespace Legolas{

//*************************************** Begin Accumulate ****************************************//
template <class ACCUMULATOR,int packLevel,int level>
struct Accumulate{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    const DERIVED & a=ba.getArrayRef();

    for (size_t i=0 ; i<a.size(); i++){
      SCALAR_TYPE aci;
      ACCUMULATOR::neutralize(aci);
      Accumulate<ACCUMULATOR,packLevel,level-1>::apply(a[i],aci);
      ACCUMULATOR::apply(aci,accumulator);
    }
  }
};

template <class ACCUMULATOR,int packLevel>
struct Accumulate<ACCUMULATOR,packLevel,packLevel>{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    const DERIVED & a=ba.getArrayRef();
    //Partie Vectorisee
    auto ap=a.getPackedView();
    typename DERIVED::PackedDoubleType packedAccumulator;
    ACCUMULATOR::neutralize(packedAccumulator);

    const size_t aps=ap.size();
    if (aps==0) return; // empty array: nothing to accumulate

    // Number of packs entirely contained in the logical size. The last pack
    // may be partial (padding) and is then handled by the scalar remainder.
    const size_t fullPacks=a.size()/DERIVED::packSize;
    const size_t vectorPacks=(fullPacks<aps) ? fullPacks : aps;

    for (size_t ip=0 ; ip<vectorPacks; ip++){
      Accumulate<ACCUMULATOR,1,packLevel-1>::apply(ap[ip],packedAccumulator);
    }

    //Accumulation du pack resultant dans un scalaire
    for (int comp=0 ; comp<DERIVED::packSize ; comp++){
      ACCUMULATOR::apply(packedAccumulator[comp],accumulator);
    }

    //Accumulation du reste (on ne passe pas dans le padding)
    for (size_t i=vectorPacks*DERIVED::packSize ; i<a.size() ; i++){
      SCALAR_TYPE aci;
      ACCUMULATOR::neutralize(aci);
      Accumulate<ACCUMULATOR,packLevel,packLevel-1>::apply(a[i],aci);
      ACCUMULATOR::apply(aci,accumulator);
    }

  }
};



template <class ACCUMULATOR, int packLevel>
struct Accumulate<ACCUMULATOR,packLevel,1>{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    auto a=ba.getArrayRef();

    const int chunkSize=400;
    const int nbChunks=a.size()/chunkSize;

    for (int j=0 ; j<nbChunks ; j++){
      SCALAR_TYPE aci;
      ACCUMULATOR::neutralize(aci);

      const int istart=j*chunkSize;
      const int iend=istart+chunkSize;
      for (int i=istart ; i<iend; i++){
        ACCUMULATOR::apply(a[i],aci);
      }
      ACCUMULATOR::apply(aci,accumulator);

    }
    for (size_t i=nbChunks*chunkSize ; i<a.size() ; i++){
      ACCUMULATOR::apply(a[i],accumulator);
    }

  }
};

template <class ACCUMULATOR>
struct Accumulate<ACCUMULATOR,1,1>{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    auto a=ba.getArrayRef();

    const int chunkSize=400;
    const int nbChunks=a.size()/chunkSize;

    for (int j=0 ; j<nbChunks ; j++){
      SCALAR_TYPE aci;
      ACCUMULATOR::neutralize(aci);

      const int istart=j*chunkSize;
      const int iend=istart+chunkSize;
      for (int i=istart ; i<iend; i++){
        ACCUMULATOR::apply(a[i],aci);
      }
      ACCUMULATOR::apply(aci,accumulator);

    }
    for (size_t i=nbChunks*chunkSize ; i<a.size() ; i++){
      ACCUMULATOR::apply(a[i],accumulator);
    }

  }
};


//*************************************** Begin Accumulate Padding ****************************************//
template <class ACCUMULATOR,int packLevel,int level>
struct AccumulatePadding{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    const DERIVED & a=ba.getArrayRef();

    for (size_t i=0 ; i<a.size(); i++){
      SCALAR_TYPE aci;
      ACCUMULATOR::neutralize(aci);
      AccumulatePadding<ACCUMULATOR,packLevel,level-1>::apply(a[i],aci);
      ACCUMULATOR::apply(aci,accumulator);
    }
  }
};

template <class ACCUMULATOR,int packLevel>
struct AccumulatePadding<ACCUMULATOR,packLevel,packLevel>{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    const DERIVED & a=ba.getArrayRef();
    //Partie Vectorisee
    auto ap=a.getPackedView();
    //typename DERIVED::PackedRealType packedAccumulator;

    const int aps=ap.size();
    const int paddingSize=aps*DERIVED::packSize-a.size();

    if ( paddingSize!=0 ){ // There is Padding !!

      const int lsy=a.size()-(aps-1)*DERIVED::packSize;

      typename DERIVED::PackedDoubleType packedAccumulator;
      ACCUMULATOR::neutralize(packedAccumulator);

      //	INFOS("aps="<<aps);

      AccumulatePadding<ACCUMULATOR,1,packLevel-1>::apply(ap[aps-1],packedAccumulator);

      //	INFOS("a.size()="<<a.size());
      //	INFOS("lsy="<<lsy);


      //Accumulation du pack resultant dans un scalaire
      for (int comp=lsy ; comp<DERIVED::packSize ; comp++){
        ACCUMULATOR::apply(packedAccumulator[comp],accumulator);
      }
    }
  }
};



template <class ACCUMULATOR, int packLevel>
struct AccumulatePadding<ACCUMULATOR,packLevel,1>{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    auto a=ba.getArrayRef();

    const int chunkSize=400;
    const int nbChunks=a.size()/chunkSize;

    for (int j=0 ; j<nbChunks ; j++){
      SCALAR_TYPE aci;
      ACCUMULATOR::neutralize(aci);

      const int istart=j*chunkSize;
      const int iend=istart+chunkSize;
      for (int i=istart ; i<iend; i++){
        ACCUMULATOR::apply(a[i],aci);
      }
      ACCUMULATOR::apply(aci,accumulator);

    }
    for (size_t i=nbChunks*chunkSize ; i<a.size() ; i++){
      ACCUMULATOR::apply(a[i],accumulator);
    }

  }
};

template <class ACCUMULATOR>
struct AccumulatePadding<ACCUMULATOR,1,1>{
  template <class DERIVED, class SCALAR_TYPE>
  static inline void apply(const BaseArray<DERIVED> & ba, SCALAR_TYPE & accumulator){
    auto a=ba.getArrayRef();

    const int chunkSize=400;
    const int nbChunks=a.size()/chunkSize;

    for (int j=0 ; j<nbChunks ; j++){
      SCALAR_TYPE aci;
      ACCUMULATOR::neutralize(aci);

      const int istart=j*chunkSize;
      const int iend=istart+chunkSize;
      for (int i=istart ; i<iend; i++){
        ACCUMULATOR::apply(a[i],aci);
      }
      ACCUMULATOR::apply(aci,accumulator);

    }
    for (size_t i=nbChunks*chunkSize ; i<a.size() ; i++){
      ACCUMULATOR::apply(a[i],accumulator);
    }

  }
};

//*************************************** End Accumulate ****************************************//



template <class LEFT, int PACK_SIZE, class RIGHT>
static inline void plusAssign(Legolas::NativeSimd<LEFT,PACK_SIZE> & a, const RIGHT & b){
  a += b;
}

static inline void plusAssign(double & a, const double & b){
  a+=b;
}


struct AddFunctor{

  template <class ELEMENT, class ACCUMULATOR_TYPE>
  static inline void apply(const ELEMENT & element,  ACCUMULATOR_TYPE & accumulator){
    //accumulator+=element;
    Legolas::plusAssign(accumulator,element);
  }

  template <class ELEMENT>
  static inline void neutralize( ELEMENT & accumulator  ){
    accumulator=0.0;
  }
};



namespace detail{

// CPU reference implementations (double accumulation). The public
// squaredNorm/dot below either call these directly or fall back to them when
// the optional GPU backend is not usable.
template <class DERIVED>
inline double squaredNormCpu(const BaseArray<DERIVED> & ba){

  const DERIVED & a=ba.getArrayRef();

  double result;
  AddFunctor::neutralize(result);
  Accumulate<AddFunctor,DERIVED::packLevel,DERIVED::level>::apply(a*a,result);


  return result;

}

template <class DERIVED>
inline double dotCpu(const BaseArray<DERIVED> & baLeft, const BaseArray<DERIVED> & baRight){
  const DERIVED & left=baLeft.getArrayRef();
  const DERIVED & right=baRight.getArrayRef();

  double result;
  AddFunctor::neutralize(result);
  Accumulate<AddFunctor,DERIVED::packLevel,DERIVED::level>::apply(left*right,result);
  return result;

}

template <class...>
struct MakeVoid { typedef void Type; };
template <class... Ts>
using VoidT = typename MakeVoid<Ts...>::Type;

// The GPU reduction kernels consume a flat float buffer. Only concrete
// level-1 float arrays match that layout without a repacking pass; lazy
// expressions and packed arrays have no scalar data pointer and stay on CPU.
template <class DERIVED, class = void>
struct GpuReductionEligible : std::false_type {};

template <class DERIVED>
struct GpuReductionEligible<
    DERIVED, VoidT<typename DERIVED::RealType,
                   decltype(std::declval<const DERIVED&>().realDataPtr())>>
    : std::integral_constant<bool, (DERIVED::level == 1) &&
                                       std::is_same<typename DERIVED::RealType,
                                                    float>::value> {};

#ifdef LEGOLAS_HAS_VULKAN
template <class DERIVED>
inline double squaredNormDispatch(const BaseArray<DERIVED> & ba,
                                  Vulkan::Backend backend, std::true_type){
  const DERIVED & a=ba.getArrayRef();
  const size_t n=static_cast<size_t>(a.size());
  if (Vulkan::shouldUseGpu(backend,n)){
    double gpuResult=0.0;
    if (Vulkan::squaredNorm(Vulkan::Context::instance(),a.realDataPtr(),n,&gpuResult)){
      return gpuResult;
    }
  }
  return squaredNormCpu(ba);
}

template <class DERIVED>
inline double squaredNormDispatch(const BaseArray<DERIVED> & ba,
                                  Vulkan::Backend, std::false_type){
  return squaredNormCpu(ba);
}

template <class DERIVED>
inline double dotDispatch(const BaseArray<DERIVED> & baLeft,
                          const BaseArray<DERIVED> & baRight,
                          Vulkan::Backend backend, std::true_type){
  const DERIVED & a=baLeft.getArrayRef();
  const DERIVED & b=baRight.getArrayRef();
  const size_t n=static_cast<size_t>(a.size());
  if (a.size()==b.size() && Vulkan::shouldUseGpu(backend,n)){
    double gpuResult=0.0;
    if (Vulkan::dot(Vulkan::Context::instance(),a.realDataPtr(),b.realDataPtr(),n,&gpuResult)){
      return gpuResult;
    }
  }
  return dotCpu(baLeft,baRight);
}

template <class DERIVED>
inline double dotDispatch(const BaseArray<DERIVED> & baLeft,
                          const BaseArray<DERIVED> & baRight,
                          Vulkan::Backend, std::false_type){
  return dotCpu(baLeft,baRight);
}
#endif

}//end of namespace detail



// Public reductions. With LEGOLAS_HAS_VULKAN the backend policy is
// Backend::Auto: the device is used when available and the workload is large
// enough, and every failure falls back to the CPU implementation.
template <class DERIVED>
inline double squaredNorm(const BaseArray<DERIVED> & ba){
#ifdef LEGOLAS_HAS_VULKAN
  return detail::squaredNormDispatch(
      ba,Vulkan::Backend::Auto,
      std::integral_constant<bool,detail::GpuReductionEligible<DERIVED>::value>());
#else
  return detail::squaredNormCpu(ba);
#endif
}



template <class DERIVED>
inline double dot(const BaseArray<DERIVED> & baLeft, const BaseArray<DERIVED> & baRight){
#ifdef LEGOLAS_HAS_VULKAN
  return detail::dotDispatch(
      baLeft,baRight,Vulkan::Backend::Auto,
      std::integral_constant<bool,detail::GpuReductionEligible<DERIVED>::value>());
#else
  return detail::dotCpu(baLeft,baRight);
#endif
}

template <class DERIVED>
inline double squaredNormZeroPad(const BaseArray<DERIVED> & ba){

  const DERIVED & a=ba.getArrayRef();

  double result;
  AddFunctor::neutralize(result);
  Accumulate<AddFunctor,DERIVED::packLevel,DERIVED::level>::apply(a*a,result);


  return result;

}

//LP: Compute the padding only part of the dot product
// It should be zero for Arrays initially filled with zeros.
// This method is used to check that dotAssumeZeroPadding is equavalent to dot (but faster)
template <class DERIVED>
inline double dotPadding(const BaseArray<DERIVED> & baLeft, const BaseArray<DERIVED> & baRight){
  const DERIVED & left=baLeft.getArrayRef();
  const DERIVED & right=baRight.getArrayRef();

  double result;
  AddFunctor::neutralize(result);
  AccumulatePadding<AddFunctor,DERIVED::packLevel,DERIVED::level>::apply(left*right,result);
  return result;

}

  
//LP: Compute the padding only part of the dot product
// It should be zero for Arrays initially filled with zeros.
// This method is used to check that dotAssumeZeroPadding is equivalent to dot (but faster)
template <class DERIVED>
inline double dotAssumeZeroPadding(const BaseArray<DERIVED> & baLeft, const BaseArray<DERIVED> & baRight){
  assert(Legolas::dotPadding(baLeft,baRight)==0.0);

  const DERIVED & left=baLeft.getArrayRef();
  const DERIVED & right=baRight.getArrayRef();
  

  
  auto lv=left.getFlatPackedView();
  auto rv=right.getFlatPackedView();
  
  const int lvsize=lv.size();

  // Accumulate a full vector across the whole loop, then perform a single
  // horizontal reduction. Doing a horizontal sum at every iteration (as the
  // original code did) serializes the reduction and defeats SIMD.
  typename DERIVED::PackedRealType psum(0.0);
  for (int i=0; i<lvsize ; i++){
    psum += lv[i] * rv[i];
  }

  double result = 0.0;
  for (int j=0; j<psum.size() ; j++){
    result += psum[j];
  }
  
  return result;
}





 

template <class DERIVED>
inline double squaredNormAssumeZeroPadding(const BaseArray<DERIVED> & ba){

  return Legolas::dotAssumeZeroPadding(ba,ba);

}

  template <class DERIVED>
inline double squaredNormPadding(const BaseArray<DERIVED> & ba){

  return Legolas::dotPadding(ba,ba);

}




// template <class SCALAR_TYPE, int LEVEL, int PACK_SIZE, int PACK_LEVEL>
// inline double dotAssumeZeroPadding(const Legolas::Array<SCALAR_TYPE,LEVEL,PACK_SIZE,PACK_LEVEL> & left,
// 				     const Legolas::Array<SCALAR_TYPE,LEVEL,PACK_SIZE,PACK_LEVEL> & right){

//   typedef Legolas::Array<SCALAR_TYPE,LEVEL,PACK_SIZE,PACK_LEVEL> AT;
//   typedef typename AT::FlatPackedArrayView FlatPackedArrayView;
//   typedef typename AT::PackedDoubleType PackedDoubleType;

//   const FlatPackedArrayView l=left.getFlatPackedView();
//   const FlatPackedArrayView r=right.getFlatPackedView();

//   PackedDoubleType accumulator(0.0);

//   for (size_t i=0 ; i<l.size() ; i++){
//     accumulator+=l[i]*r[i];
//   }

//   double result=0.0;

//   for (int comp=0 ; comp<DERIVED::packSize ; comp++){
//     result+=accumulator(comp);
//   }

//   return result;

// }





}//end of namespace Legolas
