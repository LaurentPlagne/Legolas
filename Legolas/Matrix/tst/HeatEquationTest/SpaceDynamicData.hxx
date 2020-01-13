/**
 * project  DESCARTES
 *
 * @file     SpaceDynamicData.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPACEDYNAMICDATA_HXX__
#define __LEGOLAS_SPACEDYNAMICDATA_HXX__

#include "UTILITES.hxx"

// Space model of the LayerDynamicData Concept

template <class REAL> 
class SpaceDynamicData{
  
public:

  typedef REAL RealType;

  SpaceDynamicData( void ):_meshLength(0.0),_meshSize(-1){
    MESSAGE("SpaceDynamicData Default Ctor");
  }

  SpaceDynamicData( RealType meshLength, int meshSize):_meshLength(meshLength),_meshSize(meshSize){
    MESSAGE("SpaceDynamicData Ctor");
  }

  RealType meshLength( void ) const {
    return _meshLength;
  }

  int meshSize( void ) const {
    return _meshSize;
  }

private:

  RealType _meshLength;
  int _meshSize;

};

#endif
