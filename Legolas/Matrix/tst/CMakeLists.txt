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
include ( ${thisDIR}/ConstSparseMatrixTest/CMakeLists.txt )
include ( ${thisDIR}/TransposeMatrixTest/CMakeLists.txt )
include ( ${thisDIR}/ATLASInterfaceTest/CMakeLists.txt )
include ( ${thisDIR}/DenseMatrixTest3/CMakeLists.txt )
include ( ${thisDIR}/BandedMatrixTest/CMakeLists.txt )
include ( ${thisDIR}/SparseMatrixTest/CMakeLists.txt )
include ( ${thisDIR}/PoissonEquation2Da/CMakeLists.txt )
include ( ${thisDIR}/Ex2/CMakeLists.txt )
include ( ${thisDIR}/Ex1/CMakeLists.txt )
include ( ${thisDIR}/Ex3/CMakeLists.txt )
include ( ${thisDIR}/Ex4/CMakeLists.txt )
include ( ${thisDIR}/DenseMatrixTest1/CMakeLists.txt )
include ( ${thisDIR}/DRMatrixTest/CMakeLists.txt )
include ( ${thisDIR}/GaussJordanTest/CMakeLists.txt )
include ( ${thisDIR}/TransposeMatrixMatrixTest/CMakeLists.txt )
include ( ${thisDIR}/DenseMatrixTest2/CMakeLists.txt )
include ( ${thisDIR}/DenseMatrixTest/CMakeLists.txt )
include ( ${thisDIR}/OwnershipPolicyTest/CMakeLists.txt )
include ( ${thisDIR}/HierarchyTest/CMakeLists.txt )
include ( ${thisDIR}/PoissonEquation1D/CMakeLists.txt )

dk_pop_thisDIR ()
