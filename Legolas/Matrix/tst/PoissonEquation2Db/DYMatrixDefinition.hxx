/**
 * project  DESCARTES
 *
 * @file     DYMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DYMATRIXDEFINITION_HXX__
#define __LEGOLAS_DYMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "DYData.hxx"

class DYMatrixDefinition : public Legolas::DefaultMatrixDefinition<double> {
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::TriDiagonal      MatrixStructure;
  typedef double                    RealType;
  typedef DYData                    Data;
  typedef double                    GetElement;


  static inline int getMaxIteration( const Data & data) { return 1;}
  static inline RealType getEpsilon( const Data & data) { return 1.e-6;}
  
  static inline GetElement diagonalGetElement( int i , const Data & data) { 
    return data.diagonalElement();
  }
  static inline GetElement upperDiagonalGetElement( int i , const Data & data) { 
    return data.upperDiagonalElement();
  }
  static inline GetElement lowerDiagonalGetElement( int i , const Data & data) {
    return data.upperDiagonalElement();
  }
};


#endif
    

									     

