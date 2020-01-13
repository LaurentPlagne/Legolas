/**
 * project  DESCARTES
 *
 * @file     ActualMatrixKernel.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - LP - Nettoyage - Novembre 2006
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ACTUALMATRIXKERNEL_HXX__
#define __LEGOLAS_ACTUALMATRIXKERNEL_HXX__

#include "Legolas/Matrix/ActualMatrix/ActualMatrixDataDriverTraits.hxx"
#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"

namespace Legolas{
  
//   template <class MEI, class EDD, class STORAGE_MODE>
//   struct EDDTraits{
//     typedef EDD ElementDataDriver;
//   };

//   template <class MEI, class EDD>
//   struct EDDTraits<MEI,EDD,Actual>{
//     typedef EDD ElementDataDriver;
//   };

//   template <class MEI, class EDD>
//   struct EDDTraits<MEI,EDD,Virtual>{
//     struct ElementDataDriver : public EDD{
//       typedef typename MEI::Copy Data;
//     };
//   };

  
  
  template <class MATRIX_OPTIONS, class MATRIX_ELEMENT_INTERFACE>
  class ActualMatrixKernel{
  
  private:
    
    typedef typename MATRIX_ELEMENT_INTERFACE::DataDriver                                   ElementDataDriver;
//     typedef typename MATRIX_ELEMENT_INTERFACE::DataDriver  EDD;
//     typedef typename EDD::StorageMode                      ElementStorageMode;

    
    
//     typedef typename EDDTraits<MATRIX_ELEMENT_INTERFACE,EDD,ElementStorageMode>::ElementDataDriver    ElementDataDriver;

    typedef typename MATRIX_OPTIONS::MatrixContainer                                        MatrixContainer;
    typedef typename MATRIX_OPTIONS::ActualMatrixStructure                                  MatrixStructure;
    //  typedef typename MatrixContainer::MatrixStructure                                       MatrixStructure;
    typedef typename MatrixContainer::template Engine<MATRIX_OPTIONS,ElementDataDriver>     MatrixContainerEngine;
    typedef ActualMatrixDataDriverTraits<MatrixContainerEngine,MatrixStructure,ElementDataDriver> ACMDDTraits;

  public:

    typedef MATRIX_ELEMENT_INTERFACE                      MatrixElementInterface;    
    typedef typename ACMDDTraits::Type                    MatrixDataDriver;
    typedef typename MatrixDataDriver::Data               Data;
    typedef typename MATRIX_OPTIONS::MatrixVectorProduct  MatrixVectorProduct;
    typedef typename MATRIX_OPTIONS::MatrixVectorInverse  MatrixVectorInverse;
  
  };

}

#endif


  

  
  
