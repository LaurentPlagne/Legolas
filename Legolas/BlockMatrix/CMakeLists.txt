# -*- cmake -*-
#
# @file CMakeLists.txt
#
# @project  Cocagne
#
# @version  $Id$
#
# @author Ivan DUTKA-MALEN
# @date   Thu Apr  5 14:43:44 CEST 2012
#
# @par Modifications
#
# (c) Copyright EDF R&D 2012-2013
#
dk_push_thisDIR ( ${thisDIR}/BlockMatrix )

include_directories ( ${thisDIR} )
set ( blockmatrix_SOURCEFILES
      ${thisDIR}/ConjugateGradientSolver.cxx
      ${thisDIR}/BiCGStabSolver.cxx
      ${thisDIR}/PreconditionedConjugateGradientSolver.cxx
      ${thisDIR}/SteepestDescentMinResSolver.cxx
      ${thisDIR}/SteepestDescentSolver.cxx
      ${thisDIR}/Matrix.cxx
      ${thisDIR}/VirtualSolver.cxx
      )

list ( APPEND Cocagne_SOURCEFILES ${blockmatrix_SOURCEFILES} )

include ( ${thisDIR}/Structures/CMakeLists.txt )

# Tests
include ( ${thisDIR}/tst/CMakeLists.txt )

dk_pop_thisDIR ()

