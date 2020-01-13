/**
 * project  DESCARTES
 *
 * @file     DTimeMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DTIMEMATRIXDEFINITION_HXX__
#define __LEGOLAS_DTIMEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "TimeMatrixData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"
#include "DSpaceMatrixDefinition.hxx"

template <class REAL_TYPE> 
class DTimeMatrixDefinition{
public:

  // Types that must be defined to model the MATRIX_DEFINITION concept

  typedef Legolas::Diagonal                                        MatrixStructure;
  //  typedef SPACE_MATRIX_DEFINITION                         SubMatrixDefinition;
  //  typedef typename SubMatrixDefinition::RealType          RealType;  
  //  typedef typename SubMatrixDefinition::Data              GetElement ;
  typedef REAL_TYPE                                        RealType;
  typedef TimeMatrixData<REAL_TYPE>                        Data;
  typedef typename DSpaceMatrixDefinition<REAL_TYPE>::Data GetElement;    

  
  // 2 static functions to be defined to model the DIAGONAL_MATRIX_DEFINITION concept
  
  static inline int nrows ( const Data & data ) {
    return data.nrows();
  }

  static inline GetElement zero( const Data & data){
    return Zero();
  }
  
  static inline GetElement diagonalGetElement( int i, const Data & data){
    if (data.isNull()) {
      return Zero();
    }
    else{
      return GetElement(data.deltaT());
    }
  }

  static inline int getMaxIteration( const Data & data) {
      return 1000;
  }

  static inline RealType getEpsilon( const Data & data) {
    return 1.e-6;
  }

  
};

#endif
