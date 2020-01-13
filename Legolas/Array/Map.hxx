#pragma once

#include "my_tbb_parallel_for.hxx"
#include "Legolas/Array/ForEach.hxx"

namespace Legolas{

  template <class ALGO, typename... ARRAYS>
  void scalar_map(ALGO algo, ARRAYS... rest){
    const int thisSize=getArraysSize(rest...);
    algo(0,thisSize,rest...);
  }
  template <class ALGO, typename... ARRAYS>
  void parallel_ranged_map(int begin, int end, ALGO algo, ARRAYS... rest){
    tbb::parallel_for(tbb::blocked_range<int>(begin,end),
		      [=](tbb::blocked_range<int> r){
			algo(r.begin(),r.end(),rest...);}
		      ,tbb::auto_partitioner());
			//		      });
  }


  template <bool vectorize>
  struct PackedMapSwitch{
    template <class ALGO, typename... ARRAYS>
    static inline void apply(ALGO algo, ARRAYS... rest){
      scalar_map(algo,rest...);
    }

    template <class ALGO, typename... ARRAYS>
    static inline void parallel_apply(ALGO algo, ARRAYS... rest){
      const int thisSize=getArraysSize(rest...);
      parallel_ranged_map(0,thisSize,algo,rest...);
    }
  };

  template <>
  struct PackedMapSwitch<true>{
    
    template <class ALGO, typename... ARRAYS>
    static inline void apply(ALGO algo, ARRAYS... rest){
      const int thisSize=getArraysSize(rest...);
      const int thisPackedSize=getPackedArraysSize(rest...);
      const int packSize=getPackSize(rest...);
      //LP : This is ultra hot
      algo(0,thisPackedSize,rest.getPackedView()...);
      // If rest.. is equal to a1,a2,..,an then
      // rest.getPackedView()... is equivalent to 
      //  a1.getPackedView(),a2.getPackedView(),...,an.getPackedView()
      algo(thisPackedSize*packSize,thisSize,rest...);
    }

    template <class ALGO, typename... ARRAYS>
    static inline void parallel_apply(ALGO algo, ARRAYS... rest){
      const int thisSize=getArraysSize(rest...);
      const int thisPackedSize=getPackedArraysSize(rest...);
      const int packSize=getPackSize(rest...);
      parallel_ranged_map(0,thisPackedSize,algo,rest.getPackedView()...);
      algo(thisPackedSize*packSize,thisSize,rest...);
      //      ranged_for_each(thisPackedSize*packSize,thisSize,algo,rest...);

    }
  };

      

  // template <class ALGO, typename... ARRAYS>
  // void parallel_scalar_for_each(ALGO algo, ARRAYS... rest){
  //   const int thisSize=getArraysSize(rest...);
  //   parallel_ranged_for_each(0,thisSize,algo,rest...);
  // }

  template <class ALGO, typename... ARRAYS>
  void map(ALGO algo, ARRAYS... rest){
    PackedMapSwitch<IsVectorisableArray<ARRAYS...>::result>::apply(algo,rest.getArrayRef()...);
  }

  template <class ALGO, typename... ARRAYS>
  void parmap(ALGO algo, ARRAYS... rest){
    PackedMapSwitch<IsVectorisableArray<ARRAYS...>::result>::parallel_apply(algo,rest.getArrayRef()...);
  }

  // template <class ALGO, typename... ARRAYS>
  // void flat_for_each(ALGO algo, ARRAYS... rest){
  //   scalar_for_each(algo,rest.getFlatPackedView()...);
  // }

  // template <class ALGO, typename... ARRAYS>
  // void parallel_flat_for_each(ALGO algo, ARRAYS... rest){
  //   parallel_scalar_for_each(algo,rest.getFlatPackedView()...);
  // }

  // template <class ALGO, typename... ARRAYS>
  // void parallel_for_each(ALGO algo, ARRAYS... rest){
  //   PackedMapSwitch<IsVectorisableArray<ARRAYS...>::result>::parallel_apply(algo,rest.getArrayRef()...);
  // }


  

}//namespace Legolas
