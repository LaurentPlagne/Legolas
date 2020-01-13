/**
 * project  DESCARTES
 *
 * @file     MultiDRMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_MULTIDRMATRIXDEFINITION_HXX__
#define __LEGOLAS_MULTIDRMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

template <class DR_MATRIX_INTERFACE>
class MultiDRMatrixDefinition: public Legolas::DefaultPrecision<typename DR_MATRIX_INTERFACE::DataDriver::RealType>{
public:
  // Types that must be defined to model the MATRIX_DEFINITION concept
  
  typedef Legolas::Diagonal                               MatrixStructure;
  typedef typename DR_MATRIX_INTERFACE::DataDriver::RealType          RealType;
  typedef typename DR_MATRIX_INTERFACE::DataDriver::Data  GetElement;

  typedef Legolas::MatrixShape<2>                         Data;
  
  static inline GetElement diagonalGetElement( int i , const Data & data) {
    return GetElement(data.getSubMatrixShape(i,i),data.getSubMatrixShape(i,i));
  }

};


#endif
    

									     

