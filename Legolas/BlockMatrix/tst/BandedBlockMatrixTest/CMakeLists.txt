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
dk_push_thisDIR ( ${thisDIR}/BandedBlockMatrixTest )

dk_check_test ( BlockMatrix_BandedBlockMatrixTest
  FROMSRC BandedBlockMatrixTest.cxx
  SRC MyBandedBlockMatrix.cxx MyBandedBlockMatrixLevel3.cxx

  LABELS Numerics
  IGNOREOUT )

dk_pop_thisDIR ()
