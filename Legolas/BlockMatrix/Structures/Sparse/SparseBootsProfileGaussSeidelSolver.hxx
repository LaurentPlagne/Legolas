#ifndef __LEGOLAS_SPARSEBOOTSPROFILEGAUSSSEIDELSOLVER_HXX__
#define __LEGOLAS_SPARSEBOOTSPROFILEGAUSSSEIDELSOLVER_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualSolver.hxx"

namespace Legolas{
  
  class SparseBootsProfileGaussSeidelSolver : public SparseVirtualSolver{
    VirtualVector * accumulatorPtr_;
  public:
    
    inline std::string name( void ) const { return "SparseBootsProfileGaussSeidelSolver" ;}

    void solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X);
    //    void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X);
    SparseBootsProfileGaussSeidelSolver( void );
    ~SparseBootsProfileGaussSeidelSolver( void );
    SparseBootsProfileGaussSeidelSolver(const SparseBootsProfileGaussSeidelSolver & source) ; 

    virtual VirtualSolver * clone( void ) const {
      return new SparseBootsProfileGaussSeidelSolver(*this);
    }
    
  };
}

#endif
    
