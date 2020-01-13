/**
 * project  DESCARTES
 *
 * @file     SpaceMatrix.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPACEMATRIX_HXX__
#define __LEGOLAS_SPACEMATRIX_HXX__

#include "SpaceMatrixPrivateData.hxx"

template <class REAL_TYPE>
class SpaceMatrix{
public:
  typedef REAL_TYPE RealType;
  
  typedef SpaceMatrixPrivateData<RealType> PrivateData;

  static RealType bandedGetElement( int i, int j, const PrivateData & privateData) {
    RealType value=0.0;
    if (i==j){
      value=privateData.diagonalElement();
    }
    else{
      if (privateData.timeMatrixDiagonalElement()){
	if (i==j+1){
	  value=privateData.lowerDiagonalElement();
	}
	else{
	  if (i==j-1){
	    value=privateData.upperDiagonalElement();
	  }
	}
      }
    }
    return value;
  }
  static int lsup(const PrivateData & privateData){
    int value=0;
    if (privateData.timeMatrixDiagonalElement()) value=1;
    return value;
  }
  static int linf(const PrivateData & privateData){
    int value=0;
    if (privateData.timeMatrixDiagonalElement()) value=1;
    return value;
  }
  
};

#endif
    

									     

