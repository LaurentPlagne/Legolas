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
#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

//Definition of a simple diagonal matrix with $i+1$ value on the i^{th} row diagonal element.

template <class REAL_TYPE>
class AMatrixDefinition: public Legolas::DefaultPrecision<REAL_TYPE>{
public:
  
  typedef Legolas::Dense                          MatrixStructure;
  typedef REAL_TYPE                               RealType;
  typedef REAL_TYPE                               GetElement;

  typedef Legolas::MatrixShape<1>                 Data;
  
  static inline GetElement getElement( int i , int j, const Data & data) {
//     GetElement result=0.0;
//     if ( (i==j-1) || (i==j+1) ) result= -1.0; 
//     if (i==j) result=2.0; 

    GetElement result=0.0;
    if ( (i==0) && (j==0) ) result=4.0;
    if ( (i==0) && (j==1) ) result=-2.0;

    if ( (i==1) && (j==0) ) result=-3.0;
    if ( (i==1) && (j==1) ) result=6.0;
    if ( (i==1) && (j==2) ) result=-3.0;

    if ( (i==2) && (j==1) ) result=-4.0;
    if ( (i==2) && (j==2) ) result=8.0;
    return result;
  }
  
};


#endif
    

									     

