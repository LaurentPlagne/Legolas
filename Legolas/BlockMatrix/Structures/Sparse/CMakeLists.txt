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
dk_push_thisDIR ( ${thisDIR}/Sparse )

include_directories ( ${thisDIR} )
set ( sparse_SOURCEFILES
      ${thisDIR}/SparseBlockMatrix.cxx
      ${thisDIR}/SparseGaussSeidelSolver.cxx
      ${thisDIR}/SparseMultOperator.cxx
      ${thisDIR}/SparseParallelMultOperator.cxx
      ${thisDIR}/SparseVirtualBlockMatrix.cxx
      ${thisDIR}/SparseVirtualMultOperator.cxx
      ${thisDIR}/SparseVirtualSolver.cxx
      ${thisDIR}/SparseBootsProfileGaussSeidelSolver.cxx
      ${thisDIR}/InPlaceSparseGaussSeidelSolver.cxx
      )

list ( APPEND Cocagne_SOURCEFILES ${sparse_SOURCEFILES} )


dk_pop_thisDIR ()

