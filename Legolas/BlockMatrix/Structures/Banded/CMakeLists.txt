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
dk_push_thisDIR ( ${thisDIR}/Banded )

include_directories ( ${thisDIR} )
set ( banded_SOURCEFILES
      ${thisDIR}/BandedBlockMatrix.cxx
      ${thisDIR}/BandedGaussSeidelSolver.cxx
      ${thisDIR}/BandedMultOperator.cxx
      ${thisDIR}/BandedVirtualBlockMatrix.cxx
      ${thisDIR}/BandedVirtualMultOperator.cxx
      ${thisDIR}/BandedVirtualSolver.cxx
      )

list ( APPEND Cocagne_SOURCEFILES ${banded_SOURCEFILES} )


dk_pop_thisDIR ()

