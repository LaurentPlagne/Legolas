/**
 * project  DESCARTES
 *
 * @file     XYMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_XYMATRIXDEFINITION_HXX__
#define __LEGOLAS_XYMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "YMatrixDefinition.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

class XYMatrixDefinition : public Legolas::DefaultMatrixDefinition<double>{
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::TriDiagonal       MatrixStructure;
  typedef double                     RealType;
  typedef YMatrixDefinition::Data    GetElement;    

  struct Data : public Legolas::MatrixShape<2>{
    Data(int nrows):Legolas::MatrixShape<2>(Legolas::MatrixShape<2>::Shape(nrows,nrows),
					    Legolas::MatrixShape<2>::Shape(nrows,nrows)){}
  };



  static inline GetElement diagonalGetElement( int i , const Data & data) { 
    return GetElement(data.getSubMatrixShape(i,i),true);
  }
  static inline GetElement upperDiagonalGetElement( int i , const Data & data) { 
    return GetElement(data.getSubMatrixShape(i,i+1),false);
  }
  static inline GetElement lowerDiagonalGetElement( int i , const Data & data) { 
    return GetElement(data.getSubMatrixShape(i,i-1),false);
  }



};
#endif
    

									     

