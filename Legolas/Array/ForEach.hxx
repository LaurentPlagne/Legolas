#pragma once

#include "my_tbb_parallel_for.hxx"


namespace Legolas{

  template <class ARRAY>
  inline int getArraysSize(ARRAY a){
    //    std::cout << "a.size()="<<a.size() <<std::endl;
    return a.size();
  }

  template <class ARRAY, typename... OTHER_ARRAYS>
  inline int getArraysSize(ARRAY a, OTHER_ARRAYS... other){
    //    std::cout << "a.size()="<<a.size() <<std::endl;
    assert(a.size()==getArraysSize(other...));
    return a.size();
  }


  template <class ARRAY>
  inline int getPackedArraysSize(ARRAY a){
    return a.size()/ARRAY::packSize;
  }

  template <class ARRAY, typename... OTHER_ARRAYS>
  inline int getPackedArraysSize(ARRAY a, OTHER_ARRAYS... other){
    assert(a.size()==getArraysSize(other...));
    return a.size()/ARRAY::packSize;
  }

  template <class ARRAY>
  inline int getPackSize(ARRAY a){
    return ARRAY::packSize;
  }

  template <class ARRAY, typename... OTHER_ARRAYS>
  inline int getPackSize(ARRAY a, OTHER_ARRAYS... other){
    assert(ARRAY::packSize==getPackSize(other...));
    return ARRAY::packSize;
  }


  template <class ALGO, typename... ARRAYS>
  void ranged_for_each(int begin, int end, ALGO algo, ARRAYS... rest){
    //    INFOS("begin="<<begin);
    //    INFOS("end="<<end);
    for (int i=begin ; i<end; i++){
      algo(i,rest...);
    }
  }

  // template <class ALGO>
  // void simple_for_each(int begin, int end, ALGO algo){
  //   for (int i=begin ; i<end; i++){
  //     algo(i);
  //   }
  // }



  template <class ALGO, typename... ARRAYS>
  void parallel_ranged_for_each(int begin, int end, ALGO algo, ARRAYS... rest){
    my_tbb::parallel_for(size_t(begin),size_t(end),[=](size_t i){algo(i,rest...);});
   }


  template <int PACK_LEVEL, int LEVEL>
  struct IsVectorisable{
    static const bool result=false;
  };

  template <int PACK_LEVEL>
  struct IsVectorisable<PACK_LEVEL,PACK_LEVEL>{
    static const bool result=true;
  };

  template <typename... ARRAYS>
  struct IsVectorisableArray;


  template <class ARRAY,typename... ARRAYS>
  struct IsVectorisableArray<ARRAY,ARRAYS...>{
    static const bool result=IsVectorisable<ARRAY::packLevel,ARRAY::level>::result&&IsVectorisableArray<ARRAYS...>::result;
  };

  template <>
  struct IsVectorisableArray<>{
    static const bool result=true;
  };


  template <bool vectorize>
  struct PackedForEachSwitch{
    template <class ALGO, typename... ARRAYS>
    static inline void apply(ALGO algo, ARRAYS... rest){
      const int thisSize=getArraysSize(rest...);
      ranged_for_each(0,thisSize,algo,rest...);
      //      scalar_for_each(algo,rest...);
    }

    template <class ALGO, typename... ARRAYS>
    static inline void parallel_apply(ALGO algo, ARRAYS... rest){
      const int thisSize=getArraysSize(rest...);
      parallel_ranged_for_each(0,thisSize,algo,rest...);
    }
  };

  template <>
  struct PackedForEachSwitch<true>{

    template <class ALGO, typename... ARRAYS>
    static inline void apply(ALGO algo, ARRAYS... rest){
      const int thisSize=getArraysSize(rest...);
      const int thisPackedSize=getPackedArraysSize(rest...);
      const int packSize=getPackSize(rest...);
      //LP : This is ultra hot
      ranged_for_each(0,thisPackedSize,algo,rest.getPackedView()...);
      // If rest.. is equal to a1,a2,..,an then
      // rest.getPackedView()... is equivalent to
      //  a1.getPackedView(),a2.getPackedView(),...,an.getPackedView()
      ranged_for_each(thisPackedSize*packSize,thisSize,algo,rest...);

    }

    template <class ALGO, typename... ARRAYS>
    static inline void parallel_apply(ALGO algo, ARRAYS... rest){
      const int thisSize=getArraysSize(rest...);
      const int thisPackedSize=getPackedArraysSize(rest...);
      const int packSize=getPackSize(rest...);

      parallel_ranged_for_each(0,thisPackedSize,algo,rest.getPackedView()...);
      ranged_for_each(thisPackedSize*packSize,thisSize,algo,rest...);

    }
  };


  template <class ALGO, typename... ARRAYS>
  void scalar_for_each(ALGO algo, ARRAYS... rest){
    const int thisSize=getArraysSize(rest...);
    ranged_for_each(0,thisSize,algo,rest...);
  }

  template <class ALGO, typename... ARRAYS>
  void parallel_scalar_for_each(ALGO algo, ARRAYS... rest){
    const int thisSize=getArraysSize(rest...);
    parallel_ranged_for_each(0,thisSize,algo,rest...);
  }

  template <class ALGO, typename... ARRAYS>
  void for_each(ALGO algo, ARRAYS... rest){
    PackedForEachSwitch<IsVectorisableArray<ARRAYS...>::result>::apply(algo,rest.getArrayRef()...);
  }

  template <class ALGO, typename... ARRAYS>
  void flat_for_each(ALGO algo, ARRAYS... rest){
    scalar_for_each(algo,rest.getFlatPackedView()...);
  }

  template <class ALGO, typename... ARRAYS>
  void parallel_flat_for_each(ALGO algo, ARRAYS... rest){
    parallel_scalar_for_each(algo,rest.getFlatPackedView()...);
  }

  template <class ALGO, typename... ARRAYS>
  void parallel_for_each(ALGO algo, ARRAYS... rest){
    PackedForEachSwitch<IsVectorisableArray<ARRAYS...>::result>::parallel_apply(algo,rest.getArrayRef()...);
  }




}//namespace Legolas
