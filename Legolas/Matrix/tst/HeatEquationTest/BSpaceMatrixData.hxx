/**
 * project  DESCARTES
 *
 * @file     BSpaceMatrixData.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_BSPACEMATRIXDATA_HXX__
#define __LEGOLAS_BSPACEMATRIXDATA_HXX__

#include "SpaceDynamicData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

template <class REAL_TYPE>
class BSpaceMatrixData{

public:

  typedef StaticHolder< SpaceDynamicData< REAL_TYPE > > SH;
  
  typedef REAL_TYPE RealType;
  BSpaceMatrixData( const Zero & zero ):deltaT_(0.0),
					diagonalElement_(0.0),
					nrows_(SH::layerDynamicData.meshSize())
  {
    MESSAGE("BSpaceMatrix::Data zero Ctor");
  }

  BSpaceMatrixData(RealType deltaT):deltaT_(deltaT),
				    nrows_(SH::layerDynamicData.meshSize())
  {
    
    diagonalElement_=-1.0/deltaT_;
    MESSAGE("BSpaceMatrix::Data Ctor");
  }
  
  inline int nrows( void ) const { return nrows_ ;}
  inline RealType diagonalElement( void ) const {return diagonalElement_ ;}
  
private:
  
  RealType deltaT_;
  RealType diagonalElement_;
  int nrows_;
};

#endif

  
