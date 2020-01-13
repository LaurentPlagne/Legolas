#ifndef _LEGOLAS_SEQUENTIALASSIGN_HXX__
#define _LEGOLAS_SEQUENTIALASSIGN_HXX__

namespace Legolas{

  template <class ASSIGN_MODE, class REAL_TYPE>
  struct SequentialAssign{
    
    typedef REAL_TYPE RealType;
    typedef RealType * Data;
    
    template <class DERIVED>
    static inline void apply(size_t N,
			     const DERIVED & source, 
			     Data & target,
			     int blockSize) {
      for (size_t i=0 ; i<N ; i++){
	//	target[i]+=source[i];
	ASSIGN_MODE::apply(target[i],source[i]);
      }
    }
    
  };
}

#endif 
