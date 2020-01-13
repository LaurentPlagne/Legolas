/**
 * project  DESCARTES
 *
 * @file     BTimeMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_BTIMEMATRIXDEFINITION_HXX__
#define __LEGOLAS_BTIMEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "TimeMatrixData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

template <class SPACE_MATRIX_DEFINITION> 
class BTimeMatrixDefinition{
public:
  // Types that must be defined to model the MATRIX_DEFINITION concept
  
  typedef Legolas::Banded                                          MatrixStructure;
  typedef SPACE_MATRIX_DEFINITION                         SubMatrixDefinition;
  typedef typename SubMatrixDefinition::RealType          RealType;  
  typedef typename SubMatrixDefinition::Data              GetElement ;
  typedef TimeMatrixData<RealType>                        Data;
  
  // 3 static functions to be defined to model the BANDED_MATRIX_DEFINITION concept
  
  static inline int nrows ( const Data & data ) {
    return data.nrows();
  }

  static inline GetElement zero( const Data & data){
    return Zero();
  }
  
  static inline GetElement bandedGetElement( int i, int j, const Data & data){

    Zero zero;
    GetElement result(zero);

    if ( (! data.isNull()) && (i==j+1)) result=GetElement(data.deltaT());
    //if ( (! data.isNull()) && (i==j+1)) result=GetElement(data.deltaT());
    
    return result;
  }
  static int lsup(const Data & data){
    return -1;
  }
  static int linf(const Data & data){
    return 1;
  }
  
  static inline int getMaxIteration( const Data & data) {
      return 1000;
  }

  static inline RealType getEpsilon( const Data & data) {
    return 1.e-6;
  }
  

};

#endif
