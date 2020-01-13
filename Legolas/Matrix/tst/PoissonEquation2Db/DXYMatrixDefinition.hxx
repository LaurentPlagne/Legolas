/**
 * project  DESCARTES
 *
 * @file     DXYMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DXYMATRIXDEFINITION_HXX__
#define __LEGOLAS_DXYMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "DYMatrixDefinition.hxx"

class DXYMatrixDefinition : public Legolas::DefaultMatrixDefinition<double>{
public:

  typedef Legolas::Diagonal                             MatrixStructure;
  typedef double                                        RealType;
  typedef DYMatrixDefinition::Data                      GetElement;    

  struct Data : public Legolas::MatrixShape<2>{
    Data(int nrows):Legolas::MatrixShape<2>(Legolas::MatrixShape<2>::Shape(nrows,nrows),
					    Legolas::MatrixShape<2>::Shape(nrows,nrows)){}
  };

  static inline GetElement diagonalGetElement( int i, const Data & data){
    return GetElement(data.getSubMatrixShape(i,i),false);
  }
  
  
};

#endif
