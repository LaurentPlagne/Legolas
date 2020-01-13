/**
 * project  DESCARTES
 *
 * @file     DefaultMatrixOptions.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DEFAULTMATRIXOPTIONS_HXX__
#define __LEGOLAS_DEFAULTMATRIXOPTIONS_HXX__

#include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"

namespace Legolas{

  template <class VIRTUAL_MATRIX_STRUCTURE,class PROVIDED_INPUT_MATRIX_OPTIONS>
  class DefaultMatrixOptions{
  private:
  
    typedef Legolas::InputMatrixOptions<> DefaultInputMatrixOptions;
    typedef typename DefaultTraits<DefaultInputMatrixOptions,PROVIDED_INPUT_MATRIX_OPTIONS>::Type IMO;  
    typedef typename IMO::ProvidedActualMatrixStructure ProvidedActualMatrixStructure;
    typedef typename IMO::ProvidedMatrixContainer ProvidedMatrixContainer;
  public:

    typedef VIRTUAL_MATRIX_STRUCTURE  VirtualMatrixStructure;
    typedef typename DefaultTraits<VirtualMatrixStructure,ProvidedActualMatrixStructure>::Type ActualMatrixStructure;

    typedef typename IMO::StorageMode             StorageMode;
    typedef typename IMO::MatrixVectorProduct     MatrixVectorProduct;
    typedef typename IMO::MatrixVectorInverse         MatrixVectorInverse;
    typedef typename IMO::VectorContainer         VectorContainer;
  
    typedef typename DefaultTraits<typename ActualMatrixStructure::DefaultContainer,ProvidedMatrixContainer>::Type MatrixContainer;
  
  };

}

#endif



  

    
      
