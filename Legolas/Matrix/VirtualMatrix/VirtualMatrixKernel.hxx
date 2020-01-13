/**
 * project  DESCARTES
 *
 * @file     VirtualMatrixKernel.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_VIRTUALMATRIXKERNEL_HXX__
#define __LEGOLAS_VIRTUALMATRIXKERNEL_HXX__

#include "Legolas/Matrix/VirtualMatrix/VirtualMatrixDataDriver.hxx"

namespace Legolas{

  template <class MATRIX_DEFINITION, class MATRIX_OPTIONS, class MATRIX_ELEMENT_INTERFACE>
  class VirtualMatrixKernel{
  
  private:
  
    typedef MATRIX_OPTIONS                                 MatrixOptions;
    typedef typename MatrixOptions::VirtualMatrixStructure MatrixStructure;
    typedef typename MATRIX_DEFINITION::Data               DAT;
    typedef typename MATRIX_DEFINITION::GetElement         GET;
    typedef GET                                            CGE;
    typedef typename MATRIX_ELEMENT_INTERFACE::DataDriver  EDD;
    typedef typename MatrixStructure::template Engine<MATRIX_DEFINITION,DAT,CGE,GET,EDD> Engine;
    typedef typename Engine::Hierarchy MatrixHierarchy;
  
  public:
  
    typedef MATRIX_ELEMENT_INTERFACE                                    MatrixElementInterface;
    typedef VirtualMatrixDataDriver<MatrixHierarchy,MATRIX_OPTIONS>     MatrixDataDriver;
    typedef typename MATRIX_DEFINITION::Data                            Data;
    typedef typename MatrixOptions::MatrixVectorProduct                 MatrixVectorProduct;
    typedef typename MatrixOptions::MatrixVectorInverse                     MatrixVectorInverse;
  };


}

#endif
