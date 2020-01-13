/**
 * project  DESCARTES
 *
 * @file     BTimeMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_BMATRIXDEFINITION_HXX__
#define __LEGOLAS_BMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"

#include "AMatrixDefinition.hxx"

template <class REAL_TYPE> 
class BMatrixDefinition: public Legolas::DefaultPrecision<REAL_TYPE>{
public:
  // Types that must be defined to model the MATRIX_DEFINITION concept
  
  typedef Legolas::Dense  MatrixStructure;
  typedef REAL_TYPE       RealType;
  typedef typename AMatrixDefinition<RealType>::Data GetElement;

  struct Data : public Legolas::MatrixShape<2>{
    Data(int nrows):Legolas::MatrixShape<2>(Legolas::MatrixShape<2>::Shape(nrows,nrows),
					    Legolas::MatrixShape<2>::Shape(nrows,nrows)){}
  };
  
  
  static inline GetElement getElement( int i , int j, const Data & data) {
    return GetElement(data.getSubMatrixShape(i,j));
  }

};

#endif
