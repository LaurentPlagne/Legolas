/**
 * project  DESCARTES
 *
 * @file     DenseMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DENSEMATRIXDEFINITION_HXX__
#define __LEGOLAS_DENSEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

class DenseMatrixDefinition : Legolas::DefaultPrecision<double>{
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::Dense                          MatrixStructure;
  typedef double                                  RealType;
  typedef double                                  GetElement;

  typedef Legolas::MatrixShape<1>                 Data;

  static inline GetElement getElement( int i , int j, const Data & data) { return 0.0;}
};


#endif
    

									     

