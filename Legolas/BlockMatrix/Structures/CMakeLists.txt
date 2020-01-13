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
dk_push_thisDIR ( ${thisDIR}/Structures )

include ( ${thisDIR}/ProductCompound/CMakeLists.txt )
include ( ${thisDIR}/Banded/CMakeLists.txt )
include ( ${thisDIR}/Diagonal/CMakeLists.txt )
include ( ${thisDIR}/Sparse/CMakeLists.txt )
include ( ${thisDIR}/SumCompound/CMakeLists.txt )

dk_pop_thisDIR ()

