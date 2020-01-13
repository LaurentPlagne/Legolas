/**
 * project  DESCARTES
 *
 * @file     staticObject.cxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
# include "SpaceDynamicData.hxx"
# include "TimeDynamicData.hxx" 
# include "StaticHolder.hxx"

template<> SpaceDynamicData<double> StaticHolder< SpaceDynamicData<double> >::layerDynamicData=SpaceDynamicData<double>() ;
template<> TimeDynamicData<double> StaticHolder< TimeDynamicData<double> >::layerDynamicData=TimeDynamicData<double>() ;

template<> SpaceDynamicData<float> StaticHolder< SpaceDynamicData<float> >::layerDynamicData=SpaceDynamicData<float>() ;
template<> TimeDynamicData<float> StaticHolder< TimeDynamicData<float> >::layerDynamicData=TimeDynamicData<float>() ;


