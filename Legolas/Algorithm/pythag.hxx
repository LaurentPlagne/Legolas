/**
 * project  DESCARTES
 *
 * @file     pythag.hxx
 *
 * @author Laurent PLAGNE
 * @date   November 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */

#ifndef __LEGOLAS_PYTHAG_HXX__
#define __LEGOLAS_PYTHAG_HXX__

// utilities from numerical recipes

#include <cmath>

namespace Legolas{
  using namespace std;

  template<class T>
  inline const T SQR(const T a) {return a*a;}

  template <class REAL_TYPE>
  REAL_TYPE pythag(const REAL_TYPE a, const REAL_TYPE b)
  {
    REAL_TYPE absa,absb;

    absa=fabs(a);
    absb=fabs(b);
    if (absa > absb) return absa*sqrt(1.0+SQR(absb/absa));
    else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+SQR(absa/absb)));
  }

}

#endif
