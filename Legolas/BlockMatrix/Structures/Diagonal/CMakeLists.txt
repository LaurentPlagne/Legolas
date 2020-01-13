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
dk_push_thisDIR ( ${thisDIR}/Diagonal )

include_directories ( ${thisDIR} )
set ( diagonal_SOURCEFILES
      ${thisDIR}/DiagonalBlockMatrix.cxx
      ${thisDIR}/DiagonalMultOperator.cxx
      ${thisDIR}/DiagonalSolver.cxx
      ${thisDIR}/DiagonalVirtualBlockMatrix.cxx
      ${thisDIR}/DiagonalVirtualMultOperator.cxx
      ${thisDIR}/DiagonalVirtualSolver.cxx
      )

list ( APPEND Cocagne_SOURCEFILES ${diagonal_SOURCEFILES} )


dk_pop_thisDIR ()

