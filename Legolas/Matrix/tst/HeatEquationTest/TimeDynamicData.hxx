/**
 * project  DESCARTES
 *
 * @file     TimeDynamicData.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TIMEDYNAMICDATA_HXX__
#define __LEGOLAS_TIMEDYNAMICDATA_HXX__

#include "UTILITES.hxx"

template <class REAL> 
class TimeDynamicData{
  
public:

  typedef REAL RealType;

  TimeDynamicData( void ):deltaT_(0.0),nbTimeStep_(0){
    
  }

  TimeDynamicData( REAL deltaT, int nbTimeStep):deltaT_(deltaT),nbTimeStep_(nbTimeStep){
    
  }

  inline REAL deltaT( void ) const {
    return deltaT_;
  }

  inline int nbTimeStep( void ) const {
    return nbTimeStep_;
  }


private:

  REAL deltaT_;
  int nbTimeStep_;

};

#endif
