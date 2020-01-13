/**
 * project  DESCARTES
 *
 * @file     DefaultPrecision.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DEFAULTPRECISION_HXX__
#define __LEGOLAS_DEFAULTPRECISION_HXX__

namespace Legolas{

  template <class REAL_TYPE>
  class DefaultPrecision{
  public:

    template <class DATA>
    static inline int getMaxIteration( const DATA & data) {
      return 1000;
    }

    template <class DATA>
    static inline REAL_TYPE getEpsilon( const DATA & data) {
      return 1.e-10;
    }
  };

}

#endif

  
