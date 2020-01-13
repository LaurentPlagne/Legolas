# -*- cmake -*-
#
# @file CMakeLists.txt
#
# @project  Cocagne
#
# @version  $Id$
#
# @author Yann PORA
# @date   vendredi 24 mai 2013, 10:18:57 (UTC+0200)
#
# @par Modifications
#
# (c) Copyright EDF R&D 2012-2013
#
dk_push_thisDIR ( ${thisDIR}/MultOnly )

dk_check_test ( BlockMatrix_MultOnlyMatrixTest
  FROMSRC MultOnlyMatrixTest.cxx
  CLEAN A.tex
  LABELS Numerics
  IGNOREOUT )

dk_pop_thisDIR ()
