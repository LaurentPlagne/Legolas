#ifndef _LEGOLAS_PARALLELASSIGN_HXX__
#define _LEGOLAS_PARALLELASSIGN_HXX__

//#include "AssignTraits.hxx"
#include "my_tbb_parallel_for.hxx"

namespace Legolas{

  template <class ASSIGN_MODE, class VECTOR_EXPRESSION, class REAL_TYPE>
  class AssignFunctor{
  public: 
    typedef REAL_TYPE   RealType;
    typedef RealType *  RealPtr;
    
    AssignFunctor(const VECTOR_EXPRESSION & source,
		const RealPtr target):source_(source),
				      target_(target)
    {
    }
    
    inline void operator ()(const my_tbb::blocked_range<size_t> & r) const {

      size_t b=r.begin();
      size_t e=r.end();
      for (size_t i=b ; i!=e ; i++){
	ASSIGN_MODE::apply(target_[i],source_[i]);
      }
    }
    
  private:
    const VECTOR_EXPRESSION & source_;
    const RealPtr target_;
  };


  template <class ASSIGN_MODE, class REAL_TYPE>
  struct ParallelAssign{
    
    typedef REAL_TYPE RealType;
    
    typedef RealType * Data;
    
    template <class DERIVED>
    static inline void apply(size_t N,
			     const DERIVED & source, 
			     Data & target,
			     int blockSize) {

      //      INFOS("N="<<N);

      //      INFOS("ICI");
      
      //      static my_tbb::affinity_partitioner ap; 
      //      my_tbb::parallel_for(my_tbb::blocked_range<int>(0,nblocks),CopyFunctor<ASSIGN_MODE,DERIVED,RealType>(source,target,blockSize),ap);
      
      my_tbb::parallel_for(my_tbb::blocked_range<size_t>(0,N),AssignFunctor<ASSIGN_MODE,DERIVED,RealType>(source,target));
      
    }
  };
}

#endif //__TBBBLOCKASSIGN_HXX__
