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
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"


template <class RT>
class DenseMatrixDefinition: public Legolas::DefaultMatrixDefinition<RT> {
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::Dense   MatrixStructure;
  typedef RT           RealType;
  typedef RT           GetElement;
  
  typedef Legolas::MatrixShape<1> Data;
  
  //  1 static functions to be defined to model the DENSE_MATRIX_DEFINITION concept 

  static inline GetElement getElement( int i , int j, const Data & data) { return 0.0;}
};


#endif
    

									     

