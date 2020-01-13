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
dk_push_thisDIR ( ${thisDIR}/ProductCompound )

include_directories ( ${thisDIR} )
set ( productcompound_SOURCEFILES
      ${thisDIR}/ProductCompoundMatrix.cxx
      ${thisDIR}/ProductCompoundMatrixMultOperator.cxx
      )

list ( APPEND Cocagne_SOURCEFILES ${productcompound_SOURCEFILES} )


dk_pop_thisDIR ()

