/**
 * project  DESCARTES
 *
 * @file     YMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_YMATRIXDEFINITION_HXX__
#define __LEGOLAS_YMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"

#include "YData.hxx"

class YMatrixDefinition : public Legolas::DefaultMatrixDefinition<double> {
public:
  
  //  Types that must be defined to model the MATRIX_DEFINITION concept 
  
  typedef Legolas::Banded  MatrixStructure;
  typedef double           RealType;
  typedef YData            Data;
  typedef double           GetElement;

  static inline int lsup(const Data & data) { return data.lsup();}
  static inline int linf(const Data & data) { return data.lsup();}
  
  static inline GetElement bandedGetElement( int i , int j, const Data & data) { 
    if (i==j){
      return data.diagonalElement();
    }
    else{
      return data.upperDiagonalElement();
    }
  }
  
};


#endif
    

									     

