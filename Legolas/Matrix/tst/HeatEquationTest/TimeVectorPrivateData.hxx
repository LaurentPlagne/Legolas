/**
 * project  DESCARTES
 *
 * @file     TimeVectorPrivateData.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TIMEVECTORPRIVATEDATA_HXX__
#define __LEGOLAS_TIMEVECTORPRIVATEDATA_HXX__

#include "UTILITES.hxx"
#include "TimeDynamicData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"
#include "StaticHolder.hxx"

template <class REAL_TYPE>
class TimeVectorPrivateData{
  
  typedef StaticHolder< TimeDynamicData< REAL_TYPE > > SH;
  typedef REAL_TYPE RealType;
public:
  TimeVectorPrivateData(const Zero & zero):deltaT_(0.0),
					   size_(SH::layerDynamicData.nbTimeStep())
  {
    MESSAGE("TimeVectorPrivateData zero Ctor");
  }
  
  TimeVectorPrivateData( void ):deltaT_(SH::layerDynamicData.deltaT()),
				size_(SH::layerDynamicData.nbTimeStep())
  {
    MESSAGE("TimeVectorPrivateData Ctor");
  }
  
  RealType deltaT( void ) const { return deltaT_ ; }
  int size( void ) const { return size_ ; }
  int nrows( void ) const { return size_ ; }
  
private :
  
  RealType deltaT_;
  int size_;
};

#endif
