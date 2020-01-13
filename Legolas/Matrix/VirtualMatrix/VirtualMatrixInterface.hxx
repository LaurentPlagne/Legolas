/**
 * project  DESCARTES
 *
 * @file     VirtualMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_VIRTUALMATRIXINTERFACE_HXX__
#define __LEGOLAS_VIRTUALMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/VirtualMatrix/VirtualMatrixKernel.hxx"
#include "Legolas/Matrix/MatrixSkin.hxx"

namespace Legolas{

  template <class MATRIX_DEFINITION, class MATRIX_OPTIONS, class MATRIX_ELEMENT_INTERFACE,bool IsTransposed=false>
  class VirtualMatrixInterface{
  private:
  
    typedef VirtualMatrixKernel<MATRIX_DEFINITION,MATRIX_OPTIONS,MATRIX_ELEMENT_INTERFACE> VMK;

  public:
    //  typedef 
    typedef MATRIX_DEFINITION MatrixDefinition;
    typedef typename MatrixSkin<VMK,Owner>::MatrixDataDriver   DataDriver;
    typedef MatrixSkin<VMK,Owner,IsTransposed>                 Matrix;
    typedef Matrix                                             Copy;
    typedef MatrixSkin<VMK,NotOwner,IsTransposed>              View;
    typedef MatrixSkin<VMK,ConstNotOwner,IsTransposed>         ConstView;

    typedef typename Matrix::Solver Solver;
  

//     typedef MatrixSkin<VMK,Owner,!IsTransposed>                TransposedMatrix;
//     typedef Matrix                                             TransposedView;
//     typedef Matrix                                             TransposedConstView;
//    static bool isAMatrix( void ) { return true ; }
    static const bool isAMatrix_=true;
  };

}

#endif
