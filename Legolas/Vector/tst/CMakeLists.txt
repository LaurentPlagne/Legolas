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
dk_push_thisDIR ( ${thisDIR}/tst )

# Tests
include ( ${thisDIR}/CompressedVectorTest/CMakeLists.txt )
include ( ${thisDIR}/LazyAllocTest/CMakeLists.txt )
include ( ${thisDIR}/PerformanceTest/CMakeLists.txt )
include ( ${thisDIR}/ShapeTest/CMakeLists.txt )
include ( ${thisDIR}/V1DTest/CMakeLists.txt )
include ( ${thisDIR}/V2DTest/CMakeLists.txt )
include ( ${thisDIR}/MultiDimExprTest/CMakeLists.txt )
include ( ${thisDIR}/IntVectorTest/CMakeLists.txt )
include ( ${thisDIR}/MapTest/CMakeLists.txt )
include ( ${thisDIR}/RectangularMultiVectorTest/CMakeLists.txt )
include ( ${thisDIR}/Tuto1Test/CMakeLists.txt )
include ( ${thisDIR}/FlatMultiVectortest/CMakeLists.txt )

dk_pop_thisDIR ()
