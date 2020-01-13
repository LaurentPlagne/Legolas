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
#ifndef __LEGOLAS_BMATRIXDEFINTION_HXX__
#define __LEGOLAS_BMATRIXDEFINTION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "TimeMatrixData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

template <class REAL_TYPE> 
class BTimeMatrixDefinition: public DefaultPrecision<REAL_TYPE>{
public:
  // Types that must be defined to model the MATRIX_DEFINITION concept
  
  
  typedef Dense                                           MatrixStructure;
  typedef REAL_TYPE                                       RealType;
  typedef int                                             GetElement ;
  typedef int                                             Data;
  
  // 3 static functions to be defined to model the BANDED_MATRIX_DEFINITION concept
  
  static inline int nrows ( const Data & data ) {
    return data;
  }

  static inline int ncols ( const Data & data ) {
    return data;
  }
  
  static inline GetElement zero(const Data & data) {
    return 0.0;
  }
  
  static inline GetElement getElement( int i , int j, const Data & data) {
    return data;
  }

};

#endif
