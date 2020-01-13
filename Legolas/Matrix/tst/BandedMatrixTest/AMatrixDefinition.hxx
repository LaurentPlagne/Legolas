/**
 * project  DESCARTES
 *
 * @file     AMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_AMATRIXDEFINITION_HXX__
#define __LEGOLAS_AMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "Legolas/Matrix/Helper/VoidObject.hxx"

#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

template <class REAL_TYPE>
class AMatrixDefinition : public Legolas::DefaultMatrixDefinition<REAL_TYPE>{
public:
  // Types that must be defined to model the MATRIX_DEFINITION concept
  
  typedef Legolas::Banded                         MatrixStructure;
  typedef REAL_TYPE                               RealType;
  typedef REAL_TYPE                               GetElement;

  typedef Legolas::MatrixShape<1>                 Data;

  static inline GetElement bandedGetElement( int i , int j, const Data & data) {
    return 0.0;
  }

  
  static int lsup(const Data & data){
    return 2;
  }
  static int linf(const Data & data){
    return 1;
  }
  

};


#endif
    

									     

