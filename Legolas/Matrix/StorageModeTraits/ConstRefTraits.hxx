/**
 * project  DESCARTES
 *
 * @file     ConstRefTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_CONSTREFTRAITS_HXX__
#define __LEGOLAS_CONSTREFTRAITS_HXX__

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
// It is not safe to take a & on a Virtual Matrix or Vector
// Not sure about that.. if the ref is const thi should be OK

namespace Legolas{

  template < class TYPE, class STORAGE_MODE>
  struct ConstRefTraits{
    typedef const TYPE & Type;
  };

  template < class TYPE >
  struct ConstRefTraits<TYPE,Virtual>{
    typedef const TYPE & Type;
    //    typedef TYPE Type;
  };

}

#endif
