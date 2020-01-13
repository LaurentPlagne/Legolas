/**
 * project  DESCARTES
 *
 * @file     DiagonalMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DIAGONALMATRIXDEFINITION_HXX__
#define __LEGOLAS_DIAGONALMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"

template <class RT>
class DiagonalMatrixDefinition : public Legolas::DefaultMatrixDefinition<RT> {
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::Diagonal  MatrixStructure;
  typedef RT             RealType;
  typedef RT             GetElement;
  
  typedef Legolas::MatrixShape<1> Data;

  //  2 static functions to be defined to model the DENSE_MATRIX_DEFINITION concept 

  static inline GetElement diagonalGetElement( int i , const Data & data) { return 1.0;}

};


#endif
    

									     

