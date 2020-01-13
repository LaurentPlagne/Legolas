/**
 * project  DESCARTES
 *
 * @file     VirtualMatrixDataDriver.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_VIRTUALMATRIXDATADRIVER_HXX__
#define __LEGOLAS_VIRTUALMATRIXDATADRIVER_HXX__

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"

namespace Legolas{

  template <class MATRIX_DEFINITION, class MATRIX_OPTIONS>
  class VirtualMatrixDataDriver: public MATRIX_DEFINITION
  {
  public:
    static const int level=MATRIX_DEFINITION::Data::level;

    template <class INPUT_MATRIX, class OUTPUT_MATRIX>
    static inline void copy(const INPUT_MATRIX & iM, 
			    OUTPUT_MATRIX & oM){
      //      INFOS("LAAAAAAAAAAAAAAAAAAAAA");
      //      INFOS("iM.diagonalGetElement(0)="<<iM.diagonalGetElement(0));
      oM=iM.getRef();
      //iM.getRef()=2.0;
      //      oM=2.0;
    }
    
    typedef Virtual StorageMode ;
  };

}

#endif




  
  
