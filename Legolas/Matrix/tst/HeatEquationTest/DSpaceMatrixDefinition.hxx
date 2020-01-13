/**
 * project  DESCARTES
 *
 * @file     DSpaceMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DSPACEMATRIXDEFINITION_HXX__
#define __LEGOLAS_DSPACEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "DSpaceMatrixData.hxx"
//#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"


template <class REAL_TYPE>
class DSpaceMatrixDefinition{
public:
  // Types that must be defined to model the MATRIX_DEFINITION concept
  
  typedef Legolas::TriDiagonal                             MatrixStructure;
  //  typedef RealDataDriver<REAL_TYPE>               SubMatrixDefinition;
  //  typedef typename SubMatrixDefinition::RealType  RealType;  
  //  typedef typename SubMatrixDefinition::Data      GetElement;
  typedef DSpaceMatrixData<REAL_TYPE>              Data;
  typedef REAL_TYPE                                GetElement;
  typedef REAL_TYPE                                RealType;
  //  typedef RealDataDriver<REAL_TYPE>               SubMatrixDefinition;  


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
  
  static inline GetElement upperDiagonalGetElement( int i , const Data & data) {
    return data.upperDiagonalElement();
  }
  
  static inline GetElement lowerDiagonalGetElement( int i , const Data & data) {
      return data.lowerDiagonalElement();
  }

  static inline int getMaxIteration( const Data & data) {
    return 10000;
  }

  static inline RealType getEpsilon( const Data & data) {
    return 1.e-10;
  }

  
};


#endif
    

									     

