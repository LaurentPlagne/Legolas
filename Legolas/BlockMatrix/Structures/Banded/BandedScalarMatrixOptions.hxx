#ifndef __LEGOLAS_BANDEDSCALARMATRIXOPTIONS_HXX__
#define __LEGOLAS_BANDEDSCALARMATRIXOPTIONS_HXX__

#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarOneVecContainer.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMultOperator.hxx"

namespace Legolas{

  struct BandedScalarMatrixOptions{
    typedef BandedScalarOneVecContainer       BandedContainer;
    typedef BandedScalarGaussSeidelSolver     Solver;
    typedef BandedScalarMultOperator          MultOperator;
  };
  
}

#endif
    
