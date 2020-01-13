/**
 * project  DESCARTES
 *
 * @file     APVectorDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_APVECTORDEFINITION_HXX__
#define __LEGOLAS_APVECTORDEFINITION_HXX__

#include "UTILITES.hxx"
#include "VectorOptions.hxx"
#include "APDataDriver.hxx"
#include "NoGenericVector.hxx"

template <int PRECISION>
class APVectorDefinition{
public:

  typedef NoGenericVector< APDataDriver<PRECISION> >       SubVectorDefinition;
  typedef typename SubVectorDefinition::RealType           RealType;
  typedef typename SubVectorDefinition::Data               GetElement;
  typedef int                                              Data; 
  typedef VectorOptions<Actual>                            Options;

  inline static GetElement getElement(int i, const Data & data){
    GetElement result=0;
    return result;
  }
      
  static inline int size ( const Data & data ) {
    return data;
  }
};

#endif
