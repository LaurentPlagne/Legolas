/**
 * project  DESCARTES
 *
 * @file     ConstGetElement.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_CONSTGETELEMENT_HXX__
#define __LEGOLAS_CONSTGETELEMENT_HXX__

namespace Legolas{

  template <class ELEMENT>
  class ConstGetElement{
  public:
    typedef const ELEMENT & Type;
  };
  // template <> // If the Matrix Element are integral type (double,float,int, its faster to pass the value)
  // class ConstGetElement<double>
  // {
  // public:
  //   typedef double Type;
  // };

}

#endif  
