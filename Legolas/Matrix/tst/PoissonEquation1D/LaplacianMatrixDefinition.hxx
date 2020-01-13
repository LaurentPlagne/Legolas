/**
 * project  DESCARTES
 *
 * @file     LaplacianMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_LAPLACIANMATRIXDEFINITION_HXX__
#define __LEGOLAS_LAPLACIANMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"


class LaplacianMatrixDefinition : public Legolas::DefaultMatrixDefinition<double>{
public:

  typedef Legolas::TriDiagonal      MatrixStructure;
  typedef double           RealType;
  typedef double           GetElement;

  typedef Legolas::MatrixShape<1>                 Data;

  //  3 static functions to be defined to model the MATRIX_DEFINITION concept 

  static inline int getMaxIteration( const Data & data) { return 10000;}

  static inline GetElement diagonalGetElement( int i , const Data & data) { return -2.0;}
  static inline GetElement upperDiagonalGetElement( int i , const Data & data) { return 1.0;}
  static inline GetElement lowerDiagonalGetElement( int i , const Data & data) { return 1.0;}
};


#endif
    

									     

