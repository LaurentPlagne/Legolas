/**
 * project  DESCARTES
 *
 * @file     DRMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DRMATRIXDEFINITION_HXX__
#define __LEGOLAS_DRMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "UTILITES.hxx"
#include "Legolas/Matrix/Helper/VoidObject.hxx"
#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/Helper/Zero.hxx"

#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"

namespace Legolas{

  template <class D_MATRIX_DEFINITION, class R_MATRIX_DEFINITION>
  class DRMatrixDefinition : public DefaultPrecision<typename D_MATRIX_DEFINITION::RealType>
  {
  public:

    typedef Dense                                              MatrixStructure ;
    //  typedef typename D_MATRIX_DEFINITION::SubMatrixDefinition  SubMatrixDefinition ;
    typedef typename R_MATRIX_DEFINITION::Data                 GetElement ;

    typedef D_MATRIX_DEFINITION DMatrixDefinition;
    typedef R_MATRIX_DEFINITION RMatrixDefinition;
  
    typedef typename DMatrixDefinition::Data DData;
    typedef typename RMatrixDefinition::Data RData;

    typedef typename DMatrixDefinition::RealType RealType;

    class Data{
    public:
      Data( const DData & dSource , const RData & rSource):_dData(dSource),_rData(rSource){
	MESSAGE("DRMatrixDefinition::Data Ctor");
      }
      // should be removed
      Data( const Zero & zero):_dData(zero),_rData(zero){
	MESSAGE("DRMatrixDefinition::Data Ctor");
      }

      Data( const Zero & zero, const DData & dSource, const RData & rSource  ):_dData(zero,dSource),_rData(zero,rSource){
	MESSAGE("DRMatrixDefinition::Data Ctor");
      }
      const DData & dData( void ) const {return _dData;};
      const RData & rData( void ) const {return _rData;};
    private:
      DData _dData;
      RData _rData;
    };

  };

}

#endif




