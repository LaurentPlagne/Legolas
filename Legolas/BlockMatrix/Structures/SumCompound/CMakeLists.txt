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
dk_push_thisDIR ( ${thisDIR}/SumCompound )

include_directories ( ${thisDIR} )
set (  sumcompound_SOURCEFILES
      ${thisDIR}/SumCompoundMatrix.cxx
      ${thisDIR}/SumCompoundMatrixMultOperator.cxx
      ${thisDIR}/SumCompoundSolver.cxx
      )

list ( APPEND Cocagne_SOURCEFILES ${sumcompound_SOURCEFILES} )


dk_pop_thisDIR ()

