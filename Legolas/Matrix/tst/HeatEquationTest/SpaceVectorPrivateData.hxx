/**
 * project  DESCARTES
 *
 * @file     SpaceVectorPrivateData.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPACEVECTORPRIVATEDATA_HXX__
#define __LEGOLAS_SPACEVECTORPRIVATEDATA_HXX__

#include "UTILITES.hxx"
#include "SpaceDynamicData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

template <class REAL_TYPE>
class SpaceVectorPrivateData{
public:

  typedef StaticHolder< SpaceDynamicData< REAL_TYPE > > SH;
  typedef REAL_TYPE RealType;


  SpaceVectorPrivateData(const Zero & zero ):meshLength_(0.0),
					     deltaT_(0.0),
					     size_(SH::layerDynamicData.meshSize()),
					     timeRow_(0)
  {
    MESSAGE("SpaceVectorPrivateData zero Ctor");
  }
  
  SpaceVectorPrivateData(RealType deltaT, int row):meshLength_(SH::layerDynamicData.meshLength()),
						   deltaT_(deltaT),
						   size_(SH::layerDynamicData.meshSize()),
						   timeRow_(row)
  {
    time_=RealType(timeRow_+1)*deltaT_;
    deltaX_=meshLength_/RealType(size_+1);
  }
  
  RealType time( void ) const { return time_ ; }
  RealType deltaX( void ) const { return deltaX_ ; }
  int size( void ) const { return size_ ; }

private :
  
  RealType meshLength_ ;
  RealType time_;
  RealType deltaT_;
  RealType deltaX_;
  int size_;
  int timeRow_;
};

#endif
