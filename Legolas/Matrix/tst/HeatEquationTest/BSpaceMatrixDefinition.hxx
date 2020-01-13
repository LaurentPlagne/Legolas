/**
 * project  DESCARTES
 *
 * @file     BSpaceMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_BSPACEMATRIXDEFINITION_HXX__
#define __LEGOLAS_BSPACEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "BSpaceMatrixData.hxx"
#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"

template <class REAL_TYPE>
class BSpaceMatrixDefinition{
public:
  // Types that must be defined to model the MATRIX_DEFINITION concept
  
  //  typedef RealDataDriver<REAL_TYPE>               SubMatrixDefinition;
  typedef Legolas::Diagonal                                MatrixStructure;
  //  typedef typename SubMatrixDefinition::RealType  RealType;  
  typedef REAL_TYPE                               RealType;
  typedef BSpaceMatrixData<RealType>              Data;
  typedef RealType                                GetElement;
  //  typedef typename SubMatrixDefinition::Data      GetElement;

  // 5 static functions to be defined to model the TRIDIAG_MATRIX_DEFINITION concept
  
  static inline int nrows ( const Data & data ) {
    return data.nrows();
    }
  
  static inline GetElement zero(const Data & data) {
    return 0.0;
  }

  static inline GetElement diagonalGetElement( int i , const Data & data) {
    return data.diagonalElement();
  }
  
  static inline int getMaxIteration( const Data & data) {
      return 1000;
  }

  static inline RealType getEpsilon( const Data & data) {
    return 1.e-6;
  }

  
  
};


#endif
    

									     

