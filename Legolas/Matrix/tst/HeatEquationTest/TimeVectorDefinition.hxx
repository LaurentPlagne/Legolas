/**
 * project  DESCARTES
 *
 * @file     TimeVectorDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TIMEVECTORDEFINITION_HXX__
#define __LEGOLAS_TIMEVECTORDEFINITION_HXX__

#include "UTILITES.hxx"
#include "TimeVectorPrivateData.hxx"
#include "VectorOptions.hxx"

template <class SPACE_VECTOR_DEFINITION>
class TimeVectorDefinition{

public:

  typedef SPACE_VECTOR_DEFINITION                     SubVectorDefinition;

  typedef typename SubVectorDefinition::RealType      RealType;
  typedef typename SubVectorDefinition::Data          GetElement;

  typedef TimeVectorPrivateData<RealType>             Data;

  typedef VectorOptions< Actual >                    Options;


  inline static GetElement getElement(int i, const Data & data){
    GetElement value(data.deltaT(),i);
    return value;
  }

  static inline int size ( const Data & data ) {
    return data.size();
  }
  

};

#endif

    
