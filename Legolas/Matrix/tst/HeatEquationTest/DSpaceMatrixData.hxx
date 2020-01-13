/**
 * project  DESCARTES
 *
 * @file     DSpaceMatrixData.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DSPACEMATRIXDATA_HXX__
#define __LEGOLAS_DSPACEMATRIXDATA_HXX__

#include "SpaceDynamicData.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

template <class REAL_TYPE>
class DSpaceMatrixData{

public:

  typedef StaticHolder< SpaceDynamicData< REAL_TYPE > > SH;
  
  typedef REAL_TYPE RealType;

  DSpaceMatrixData( void ):meshLength_(0.0),
			   deltaT_(0.0),
			   diagonalElement_(0.0),
			   upperDiagonalElement_(0.0),
			   lowerDiagonalElement_(0.0),
			   nrows_(0)
  {
  }



  DSpaceMatrixData( const Zero & zero ):meshLength_(0.0),
					deltaT_(0.0),
					diagonalElement_(0.0),
					upperDiagonalElement_(0.0),
					lowerDiagonalElement_(0.0),
					nrows_(SH::layerDynamicData.meshSize())
  {
    MESSAGE("DSpaceMatrix::Data zero Ctor");
  }

  DSpaceMatrixData(RealType deltaT):meshLength_(SH::layerDynamicData.meshLength()),
				    deltaT_(deltaT),
				    nrows_(SH::layerDynamicData.meshSize())
  {
    
    RealType deltaX = meshLength_/RealType(nrows_+1) ;
    RealType a=1.0 / ( deltaX*deltaX) ;
    diagonalElement_=2.0*a+(1.0/deltaT_);
    
    upperDiagonalElement_=-1.0*a;
    lowerDiagonalElement_=-1.0*a;
    MESSAGE("DSpaceMatrix::Data Ctor");
  }
  
  inline int nrows( void ) const { return nrows_ ;}
  inline RealType diagonalElement( void ) const {return diagonalElement_ ;}
  inline RealType upperDiagonalElement( void ) const {return upperDiagonalElement_ ;}
  inline RealType lowerDiagonalElement( void ) const {return lowerDiagonalElement_ ;}
  
private:
  
  RealType meshLength_ ;
  RealType deltaT_;
  RealType diagonalElement_;
  RealType upperDiagonalElement_;
  RealType lowerDiagonalElement_;
  int nrows_;
};

#endif

  
