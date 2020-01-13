/**
 * project  DESCARTES
 *
 * @file     GetElementTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_GETELEMENTTRAITS_HXX__
#define __LEGOLAS_GETELEMENTTRAITS_HXX__

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/OwnerShipPolicy.hxx"

// IF      StorageMode==Virtual THEN GetElement=ConstGetElement=MatrixElement::Copy
// ELSE GetElement=MatrixElement::View ; ConstGetElement=MatrixElement::ConstView       

namespace Legolas{

  template <class MATRIX_ELEMENT, template <class> class OWNERSHIP_POLICY, class STORAGE_MODE>
  class GetElementTraits{
  public:
    typedef typename MATRIX_ELEMENT::View      GetElement;
    typedef typename MATRIX_ELEMENT::ConstView ConstGetElement;
  };
  // If the Matrix Container is Virtual, the Elements must Own their Data
  template <class MATRIX_ELEMENT, template <class> class OWNERSHIP_POLICY> 
  class GetElementTraits<MATRIX_ELEMENT,OWNERSHIP_POLICY,Virtual>
  {
  public:
    typedef typename MATRIX_ELEMENT::Copy GetElement;
    typedef typename MATRIX_ELEMENT::Copy ConstGetElement;
  };
  // If the Matrix Container is a ConstNotOwner it Can't produce a View...
  template <class MATRIX_ELEMENT> 
  class GetElementTraits<MATRIX_ELEMENT,ConstNotOwner,Actual>
  {
  public:
    typedef typename MATRIX_ELEMENT::ConstView GetElement;
    typedef typename MATRIX_ELEMENT::ConstView ConstGetElement;
  };

}

#endif 
