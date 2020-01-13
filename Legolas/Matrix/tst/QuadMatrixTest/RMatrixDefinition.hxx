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
#ifndef __LEGOLAS_RMATRIXDEFINITION_HXX__
#define __LEGOLAS_RMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"


template <class REAL_TYPE>
class RMatrixDefinition: public Legolas::DefaultPrecision<REAL_TYPE>{
public:
  
  typedef Legolas::Dense                          MatrixStructure;
  typedef REAL_TYPE                               RealType;
  typedef REAL_TYPE                               GetElement;
  typedef Legolas::MatrixShape<1>                 Data;


  static inline GetElement getElement( int i , int j, const Data & data) {
    GetElement result=0.0;
    if ( (i==j-1) || (i==j+1) ) result= -1.0; 
    if (i==j) result=2.0; 
    return result;
  }


  
};


#endif
    

									     

