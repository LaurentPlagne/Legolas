/**
 * project  DESCARTES
 *
 * @file     ActualMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ACTUALMATRIXINTERFACE_HXX__
#define __LEGOLAS_ACTUALMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/ActualMatrix/ActualMatrixKernel.hxx"
#include "Legolas/Matrix/MatrixSkin.hxx"
#include "Legolas/Matrix/ActualMatrix/ActualMatrixCopy.hxx"

namespace Legolas{

  template <class MATRIX_OPTIONS, class MATRIX_ELEMENT_INTERFACE, bool IsTransposed=false>
  class ActualMatrixInterface{
  public:
  
    typedef ActualMatrixKernel<MATRIX_OPTIONS,MATRIX_ELEMENT_INTERFACE> AMK;

    typedef ActualMatrixCopy<AMK,IsTransposed>                 Matrix;
    typedef Matrix                                             Copy;
    typedef MatrixSkin<AMK,NotOwner,IsTransposed>              View;
    typedef MatrixSkin<AMK,ConstNotOwner,IsTransposed>         ConstView;

    typedef MatrixSkin<AMK,NotOwner,!IsTransposed>             TransposedView;
    typedef MatrixSkin<AMK,ConstNotOwner,!IsTransposed>        TransposedConstView;

    typedef typename Copy::MatrixDataDriver                    DataDriver;
    static bool isAMatrix( void ) { return true ; }
    static const bool isAMatrix_=true;

    typedef typename Matrix::Solver Solver;
  
  };

}


#endif
