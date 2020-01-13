/**
 * project  DESCARTES
 *
 * @file     Zero.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ZERO_HXX__
#define __LEGOLAS_ZERO_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  class Zero{
  public:
    Zero( void ){
      MESSAGE ("Zero Ctor");
    }

    operator double ( void ){
      MESSAGE ("Zero double conversion operator");
      return 0.0;
    }

    operator float ( void ){
      MESSAGE ("Zero float conversion operator");
      return 0.0;
    }
  };

}

#endif
  
  
  
  
