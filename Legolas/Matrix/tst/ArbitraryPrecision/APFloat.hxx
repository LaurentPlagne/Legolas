/**
 * project  DESCARTES
 *
 * @file     APFloat.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_APFLOAT_HXX__
#define __LEGOLAS_APFLOAT_HXX__

#include "apfloat.h"
#include "apcplx.h"

template <int PRECISION>
class APFloat : public apfloat
{
public:
  
  APFloat( void ):apfloat(0,PRECISION){};
  template <class SOURCE>
  APFloat(const SOURCE & source):apfloat(source){
    (*this).prec(PRECISION);
  };

  
};

#endif
  
