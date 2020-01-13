#pragma once

#define LOG2_OF_BLOCKSIZE 10

namespace Legolas{

  
  template <class L>
  class SetZeroFunctor{
    L & left_;
  public:
    SetZeroFunctor(L & left):left_(left){}
    
    inline void operator()(size_t begin, size_t length) const {
      left_.sequential_nullify(begin,length);
    }
  };



  template <class L, class ASSIGN_MODE, class R>
  class BinaryRangeFunctor{
    L & left_;
    const R & right_;
  public:
    BinaryRangeFunctor(L & left, const R & right):left_(left),right_(right){}
    
    inline void operator()(size_t begin, size_t length) const {
#ifdef LEGOLAS_USE_EIGEN
      ASSIGN_MODE::applyOnViews(left_.eigenVectorView(begin,length),right_.eigenVectorView(begin,length));
#else
      const size_t end=begin+length;
      for (size_t i=begin ; i<end ; i++){
	ASSIGN_MODE::apply(left_(i),right_(i));
      }
#endif
    }
  };


  template <class L>
  class BinaryRangeFunctor<L,EqualAssign,L>{
    L & left_;
    const L & right_;
  public:
    BinaryRangeFunctor(L & left, const L & right):left_(left),right_(right){}
    
    inline void operator()(size_t begin, size_t length) const {
#ifdef LEGOLAS_USE_EIGEN
      EqualAssign::applyOnViews(left_.eigenVectorView(begin,length),right_.eigenVectorView(begin,length));
#else
      left_.sequential_copy(right_,begin,length);
#endif
    }
  };




  
  template <class ASSIGN_MODE>
  class BinaryRangeFunctorFactory{
  public:
    //    template <class L, class R>
    //    static auto apply(L & left, const R & right)->decltype(BinaryRangeFunctor<L,ASSIGN_MODE,R>(left,right)){
    //      return BinaryRangeFunctor<L,ASSIGN_MODE,R>(left,right);
    //    }

    template <class L, class R>
    static inline BinaryRangeFunctor<L,ASSIGN_MODE,R> apply(L & left, const R & right){
      return BinaryRangeFunctor<L,ASSIGN_MODE,R>(left,right);
    }

  };


  // template <class L, class ASSIGN_MODE, class REAL_TYPE>
  // class ScalarAssignRangeFunctor{
  //   L & left_;
  //   REAL_TYPE value_;
  // public:
  //   UnaryRangeFunctor(L & left, const REAL_TYPE & value):left_(left),value_(value){}
    
  //   inline void operator()(size_t begin, size_t length) const {
  //     ASSIGN_MODE::applyOnViewsWithScalar(left_.eigenVectorView(begin,length),value_);
  //   }
  // };

  
  template <class SCALAR>
  class ScalarAdapter{
    SCALAR scalar_;
  public:
    ScalarAdapter( const SCALAR & scalar ):scalar_(scalar){}
#ifdef LEGOLAS_USE_EIGEN
    inline const SCALAR & eigenVectorView(size_t begin, size_t end) const { return scalar_;}
#endif
    inline const SCALAR & operator()(size_t i) const { return scalar_ ;}
    
  };



  // class ForAll{
  // public:
    
  //   template <class RANGE_FUNCTOR>
  //   inline void apply(const RANGE_FUNCTOR & rangeFunctor,size_t begin, size_t length){
  //     rangeFunctor(begin,length);
  //   }
  // };




  template <class RANGE_FUNCTOR>
  class BlockFunctor{
    const RANGE_FUNCTOR & rangeFunctor_;
  public:
    BlockFunctor(const RANGE_FUNCTOR & rangeFunctor):rangeFunctor_(rangeFunctor){}
    void operator()(const my_tbb::blocked_range<size_t>& r) const {
      const size_t b=r.begin();
      const size_t e=r.end();
      const size_t begin=b<<LOG2_OF_BLOCKSIZE;
      const size_t length=(e-b)<<LOG2_OF_BLOCKSIZE;
      rangeFunctor_(begin,length);
    }
  };
  
  
  class ParallelBlockedForAll{
  public:
    
    template <class RANGE_FUNCTOR>
    static inline void apply(const RANGE_FUNCTOR & rangeFunctor,size_t begin, size_t length){
      
      if (length>100000){
	
	const size_t nbBlocks=length>>LOG2_OF_BLOCKSIZE;
	
	//blocked Part
	BlockFunctor<RANGE_FUNCTOR> blockFunctor(rangeFunctor);
	
	my_tbb::parallel_for(my_tbb::blocked_range<size_t>(0,nbBlocks), blockFunctor);
	
	//reminder
	const size_t smallBlockBegin=nbBlocks<<LOG2_OF_BLOCKSIZE;
	if (smallBlockBegin<length){
	  const size_t smallBlockLength=length-smallBlockBegin;
	  rangeFunctor(smallBlockBegin,smallBlockLength);
	}
      }
      else{
       	rangeFunctor(begin,length);
      }
      
    }
  };


  class SequentialForAll{
  public:
    
    template <class RANGE_FUNCTOR>
    static inline void apply(const RANGE_FUNCTOR & rangeFunctor,size_t begin, size_t length){
      rangeFunctor(begin,length);
    }
  };

}

  

    
