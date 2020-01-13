#ifndef __LEGOLAS_SPARSESCALARMATRIXOPTIONS_HXX__
#define __LEGOLAS_SPARSESCALARMATRIXOPTIONS_HXX__

#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarVecMapContainer.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMultOperator.hxx"

namespace Legolas{

  struct SparseScalarMatrixOptions{
    typedef SparseScalarVecMapContainer       SparseContainer;
    typedef SparseScalarGaussSeidelSolver     Solver;
    typedef SparseScalarMultOperator          MultOperator;
  };
  
}

#endif
    
