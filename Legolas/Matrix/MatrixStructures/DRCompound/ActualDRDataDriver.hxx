/**
 * project  DESCARTES
 *
 * @file     ActualDRDataDriver.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ACTUALDRDATADRIVER_HXX__
#define __LEGOLAS_ACTUALDRDATADRIVER_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  template <class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE>
  class ActualDRDataDriver{
  public:
  
    typedef D_MATRIX_INTERFACE                    DMatrixInterface;
    typedef R_MATRIX_INTERFACE                    RMatrixInterface;
    typedef typename DMatrixInterface::DataDriver DDataDriver;    
    typedef typename RMatrixInterface::DataDriver RDataDriver;    
    typedef typename DDataDriver::Data            DData;
    typedef typename RDataDriver::Data            RData;
  
    typedef typename DDataDriver::RealType        RealType;    

    static const int level=DDataDriver::level;

    class Data{
    public:
      Data(const DData & dData, const RData & rData)
	:_dData(dData),_rData(rData)
      {
	MESSAGE("DRMatrixInterface::DataDriver::Data Ctor");
      }
      Data( void )
	:_dData(),_rData()
      {
	MESSAGE("DRMatrixInterface::DataDriver::Data Ctor");
      }
      Data( const Data& data )
	: _dData(data._dData)
	, _rData(data._rData)
      {
	MESSAGE("DRMatrixInterface::DataDriver::Data Ctor");
      }
      DData & dData( void ) {return _dData;};
      RData & rData( void ) {return _rData;};
      const DData & dData( void ) const {return _dData;};
      const RData & rData( void ) const {return _rData;};
    private:
    
      DData _dData;
      RData _rData;
    };
  
    template< class INPUT_DR_MATRIX >
    static inline void copy(const INPUT_DR_MATRIX & inputMatrix, Data & matrix){
      DDataDriver::copy(inputMatrix.D(),matrix.dData());
      RDataDriver::copy(inputMatrix.R(),matrix.rData());
    
    }
  
  };

}

#endif
