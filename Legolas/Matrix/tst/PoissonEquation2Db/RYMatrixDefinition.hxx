/**
 * project  DESCARTES
 *
 * @file     RYMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_RYMATRIXDEFINITION_HXX__
#define __LEGOLAS_RYMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "RYData.hxx"

class RYMatrixDefinition : public Legolas::DefaultMatrixDefinition<double> {
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::Diagonal   MatrixStructure;
  typedef double              RealType;
  typedef RYData              Data;
  typedef double              GetElement;

  //  1 static functions to be defined to model the DIAGONAL_MATRIX_DEFINITION concept 

  static inline GetElement diagonalGetElement( int i , const Data & data) { 
    return data.diagonalElement();
  }

};


#endif
    

									     

