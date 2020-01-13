/**
 * project  DESCARTES
 *
 * @file     SpaceSourceVectorDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPACESOURCEVECTORDEFINITION_HXX__
#define __LEGOLAS_SPACESOURCEVECTORDEFINITION_HXX__

#include "UTILITES.hxx"
#include "SpaceVectorPrivateData.hxx"
#include "VectorOptions.hxx"
#include "NoGenericVector.hxx"

template <class REAL_TYPE>
class SpaceSourceVectorDefinition{

public:

  typedef NoGenericVector< RealDataDriver<REAL_TYPE> > SubVectorDefinition;


  typedef typename SubVectorDefinition::RealType      RealType;
  typedef typename SubVectorDefinition::Data          GetElement;

  typedef SpaceVectorPrivateData<RealType>            Data;

  typedef VectorOptions< Actual >                    Options;


  inline static GetElement getElement(int i, const Data & data){
    RealType x=RealType(i+1)*data.deltaX();
    RealType t=data.time();
    RealType value=x*cos(x)*cos(t)+2*sin(x)*sin(t)+x*cos(x)*sin(t) ;
    //RealType value=x*cos(x)*cos(t)-2*sin(x)*sin(t)-x*cos(x)*sin(t) ;
    return value;
  }

  static inline int size ( const Data & data ) {
    return data.size();
  }
  

};

#endif

    
