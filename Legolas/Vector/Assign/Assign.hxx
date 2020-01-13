#ifndef _LEGOLAS_ASSIGN_HXX__
#define _LEGOLAS_ASSIGN_HXX__

//#include "Legolas/Vector/AssignTraits.hxx"
//#include "Legolas/Vector/TBBBlockAssign.hxx"
#include "Legolas/Vector/Assign/ParallelAssign.hxx"
#include "Legolas/Vector/Assign/SequentialAssign.hxx"

namespace Legolas{
  
  template <class ASSIGN_MODE, class ELEMENT>
  struct Assign{
    
    typedef ELEMENT * Data;
    
    static const int largeSize=10000000;
    static const int blockSize=1024;
    
    template <class SOURCE>
    static inline void apply(size_t N, const SOURCE & source, Data target) {
      
      //      INFOS("N="<<N);
      
      
      if (N<size_t(largeSize)){
	//AssignTraits<ASSIGN_MODE>::applyLoop(0,N,source,target);
	SequentialAssign<ASSIGN_MODE,ELEMENT>::apply(N,source,target,blockSize);
      }
      else{
	ParallelAssign<ASSIGN_MODE,ELEMENT>::apply(N,source,target,blockSize);
	//TBBBlockAssign<ASSIGN_MODE,ELEMENT>::apply(N,source,target,blockSize);
      }
    }
  };

}

#endif
