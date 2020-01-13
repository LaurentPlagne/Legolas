#ifndef __LEGOLAS_TRIDIAGONALSCALARMATRIXOPTIONS_HXX__
#define __LEGOLAS_TRIDIAGONALSCALARMATRIXOPTIONS_HXX__

#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarContainer.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarDirectSolver.hxx"
#include "Legolas/BlockMatrix/Structures/TriDiagonal/TriDiagonalScalarMultOperator.hxx"

namespace Legolas{

  struct TriDiagonalScalarMatrixOptions{
    typedef TriDiagonalScalarContainer             TriDiagonalContainer;
    typedef TriDiagonalScalarGaussSeidelSolver     Solver;
    //typedef TriDiagonalScalarDirectSolver     Solver;
    typedef TriDiagonalScalarMultOperator          MultOperator;
  };
  
}

#endif
    
