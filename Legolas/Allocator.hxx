#pragma once

#include "dkconfig.hxx"

#if ! defined (USING_TBB)
#error C macro USING_TBB is not defined
#endif

#if USING_TBB == 1

//#warning  USING_TBB est positionnée a 1
#include "tbb/cache_aligned_allocator.h"

#elif USING_TBB == 0

#include <memory>

#endif

#include <Eigen/Core>

namespace Legolas{

#if USING_TBB == 1
template <class T> using allocator=tbb::cache_aligned_allocator<T>;
#elif  USING_TBB == 0
template <class T> using allocator=Eigen::aligned_allocator<T>;
#endif

}
