/**
 * project  DESCARTES
 *
 * @file     APDataDriver.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_APDATADRIVER_HXX__
#define __LEGOLAS_APDATADRIVER_HXX__

#include "APFloat.hxx"

template <int PRECISION>
class APDataDriver{
public:
  
  typedef APFloat<PRECISION> RealType;
  typedef RealType           Data;

  static inline void copy( const Data & input, Data & data){
    data=input;
  }
  static inline RealType dot( const RealType & left, const RealType & right){
    return left*right;
  }
  
};

#endif
  
