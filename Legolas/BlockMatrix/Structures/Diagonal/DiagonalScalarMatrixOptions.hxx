#ifndef __LEGOLAS_DIAGONALSCALARMATRIXOPTIONS_HXX__
#define __LEGOLAS_DIAGONALSCALARMATRIXOPTIONS_HXX__

#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarContainer.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalScalarMultOperator.hxx"

namespace Legolas{

  struct DiagonalScalarMatrixOptions{
    typedef DiagonalScalarContainer             DiagonalContainer;
    typedef DiagonalScalarSolver                Solver;
    typedef DiagonalScalarMultOperator          MultOperator;
  };
  
}

#endif
    
