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
#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

class DiagonalMatrixDefinition : public Legolas::DefaultPrecision<double>{
public:

  typedef Legolas::Diagonal  MatrixStructure;
  typedef double             RealType;
  typedef double             GetElement;

  typedef Legolas::MatrixShape<1>   Data;

  static inline GetElement diagonalGetElement( int i , const Data & data) { return 1.0;}

};


#endif
    

									     

