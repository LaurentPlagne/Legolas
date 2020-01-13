/**
 * project  DESCARTES
 *
 * @file     TimeMatrixData.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TIMEMATRIXDATA_HXX__
#define __LEGOLAS_TIMEMATRIXDATA_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/Helper/VoidObject.hxx"
#include "TimeDynamicData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

template <class REAL_TYPE>
class TimeMatrixData{
  
public:

  typedef StaticHolder< TimeDynamicData< REAL_TYPE > > SH;
  
  typedef REAL_TYPE RealType;

  TimeMatrixData( const Zero & zero ):deltaT_(0.0),
				       size_(SH::layerDynamicData.nbTimeStep()),
				       isNull_(true)
  {
    MESSAGE("TimeMatrixData zero Ctor");
  }
  
  TimeMatrixData(const VoidObject & voidObject):deltaT_(SH::layerDynamicData.deltaT()),
						 size_(SH::layerDynamicData.nbTimeStep()),
						 isNull_(false)
  {
    MESSAGE("TimeMatrixData Ctor");
  }
  
  RealType deltaT( void ) const { return deltaT_ ; }
  int size( void ) const { return size_ ; }
  int nrows( void ) const { return size_ ; }
  bool isNull( void ) const { return isNull_ ; }
  
private :
  
  RealType deltaT_;
  int size_;
  bool isNull_;

};

#endif
