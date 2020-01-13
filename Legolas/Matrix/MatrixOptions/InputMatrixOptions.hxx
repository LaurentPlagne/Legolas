/**
 * project  DESCARTES
 *
 * @file     InputMatrixOptions.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_INPUTMATRIXOPTIONS_HXX__
#define __LEGOLAS_INPUTMATRIXOPTIONS_HXX__

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/Helper/Default.hxx"
#include "Legolas/Matrix/GenericVector/STLVectorContainer.hxx"
#include "Legolas/Matrix/GenericVector/CVectorContainer.hxx"
#include "Legolas/Algorithm/DiagonalMatrixInversion.hxx"
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"
#include "Legolas/Algorithm/SparseGaussSeidel.hxx"
#include "Legolas/Algorithm/NonPreconditionedLinearBiConjugateGradientAlgorithm.hxx"

namespace Legolas{

  template <class STORAGE_MODE=Virtual,
	    class MATRIX_VECTOR_INV= NonPreconditionedLinearBiConjugateGradientAlgorithm, //SparseGaussSeidel,
	    class MATRIX_VECTOR_PRODUCT=SparseMatrixVectorProduct,
	    class VECTOR_CONTAINER=STLVectorContainer,
	    class PROVIDED_MATRIX_CONTAINER=Default,
	    class PROVIDED_ACTUAL_MATRIX_STRUCTURE=Default>
  class InputMatrixOptions{
  public:
  
    typedef STORAGE_MODE                       StorageMode;
    typedef MATRIX_VECTOR_INV                  MatrixVectorInverse;
    typedef MATRIX_VECTOR_PRODUCT              MatrixVectorProduct;
    typedef VECTOR_CONTAINER                   VectorContainer;
    typedef PROVIDED_MATRIX_CONTAINER          ProvidedMatrixContainer;
    typedef PROVIDED_ACTUAL_MATRIX_STRUCTURE   ProvidedActualMatrixStructure;

  
  };

//   template <class STORAGE_MODE=Virtual,
// 	    class MATRIX_VECTOR_INV=SparseMatrixInversionByGaussSeidelAlgorithm,
// 	    class MATRIX_VECTOR_PRODUCT=SparseMatrixVectorProduct,
// 	    class VECTOR_CONTAINER=STLVectorContainer,
// 	    class PROVIDED_MATRIX_CONTAINER=Default,
// 	    class PROVIDED_ACTUAL_MATRIX_STRUCTURE=Default>
//   class InputMatrixOptions{
//   public:
  
//     typedef STORAGE_MODE                       StorageMode;
//     typedef MATRIX_VECTOR_INV                  MatrixVectorInverse;
//     typedef MATRIX_VECTOR_PRODUCT              MatrixVectorProduct;
//     typedef VECTOR_CONTAINER                   VectorContainer;
//     typedef PROVIDED_MATRIX_CONTAINER          ProvidedMatrixContainer;
//     typedef PROVIDED_ACTUAL_MATRIX_STRUCTURE   ProvidedActualMatrixStructure;

  
//   };

}

#endif



  

    
      
