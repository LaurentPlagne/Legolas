#ifndef __LEGOLAS_BLOCKLINEARSOLVERTRAITS_HXX__
#define __LEGOLAS_BLOCKLINEARSOLVERTRAITS_HXX__

#include "Legolas/Algorithm/PolymorphicSolver.hxx"
#include "Legolas/Matrix/PolymorphicMatrix.hxx"

namespace Legolas{

  template <class REAL_TYPE>
  struct RealSolver{
    RealSolver(const REAL_TYPE & aii, REAL_TYPE & xi, const REAL_TYPE & bi){}

    inline void solve(const REAL_TYPE & aii, REAL_TYPE & xi, const REAL_TYPE & bi){
      xi=bi/aii;
    }

    inline void transposeSolve(const REAL_TYPE & aii, REAL_TYPE & xi, const REAL_TYPE & bi){
      xi=bi/aii;
    }
  };

  
  template <class ETA>
  class BlockLinearSolverTraits{
  public:
    typedef typename ETA::Solver Type;
  };

  template <>
  struct BlockLinearSolverTraits<const double &>{
    typedef RealSolver<double> Type;
  };

  template <>
  struct BlockLinearSolverTraits<float>{
    typedef RealSolver<float> Type;
  };

}

#endif

    
    
  




