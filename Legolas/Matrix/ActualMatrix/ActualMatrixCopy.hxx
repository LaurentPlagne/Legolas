/**
 * project  DESCARTES
 *
 * @file     ActualMatrixCopy.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ACTUALMATRIXCOPY_HXX__
#define __LEGOLAS_ACTUALMATRIXCOPY_HXX__

#include "Legolas/Matrix/MatrixSkin.hxx"

namespace Legolas{

  template <class ACTUAL_MATRIX_KERNEL, bool IsTransposed=false>
  class ActualMatrixCopy: public MatrixSkin<ACTUAL_MATRIX_KERNEL,Owner>{
  public:

    typedef typename ACTUAL_MATRIX_KERNEL::MatrixDataDriver::Data Data ;
    typedef  MatrixSkin<ACTUAL_MATRIX_KERNEL,Owner,IsTransposed> MS;

    ActualMatrixCopy(const Data & init):MS(init){
      MESSAGE("ActualMatrixCopy Ctor");
    }

    ActualMatrixCopy( void ):MS(){
      MESSAGE("ActualMatrixCopy Default Ctor");
    }

    template <class INPUT_MATRIX>
    ActualMatrixCopy( const INPUT_MATRIX & inputMatrix):MS(Data()){
      typedef typename ACTUAL_MATRIX_KERNEL::MatrixDataDriver MatrixDataDriver;
      MatrixDataDriver::copy(inputMatrix,this->getRef());
    }

    typedef ActualMatrixCopy<ACTUAL_MATRIX_KERNEL,IsTransposed> Copy;

    typedef typename MS::StorageMode StorageMode;
    typedef typename ConstRefTraits<ActualMatrixCopy,StorageMode>::Type StoreType;
    
  };

}
#endif  
