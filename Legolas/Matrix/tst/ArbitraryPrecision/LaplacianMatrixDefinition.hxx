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
#include "APFloat.hxx"
template <int PRECISION>
class LaplacianMatrixDefinition {
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::TriDiagonal                 MatrixStructure;
  typedef APFloat<PRECISION>          RealType;
  typedef int                         Data;
  typedef RealType                    GetElement;

public:

  //  3 static functions to be defined to model the MATRIX_DEFINITION concept 
  
  static inline int getMaxIteration( const Data & data) { return 10000;}
  static inline RealType getEpsilon( const Data & data) { return 1.e-10;}
  static inline int nrows ( const Data & data ) { return data;}
  
  //  static functions to be defined to model the TRIDIAGONAL_MATRIX_DEFINITION concept 

  static inline GetElement zero(const Data & data) { return 0;}
  
  static inline GetElement diagonalGetElement( int i , const Data & data) { return -2;}
  static inline GetElement upperDiagonalGetElement( int i , const Data & data) { return 1;}
  static inline GetElement lowerDiagonalGetElement( int i , const Data & data) { return 1;}
};


#endif
    

									     

