/**
 * project  DESCARTES
 *
 * @file     ActualDRMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ACTUALDRMATRIXINTERFACE_HXX__
#define __LEGOLAS_ACTUALDRMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/MatrixStructures/DRMatrix/ActualDRDataDriver.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/ActualDRMatrix.hxx"
#include "Legolas/Matrix/OwnerShipPolicy.hxx"
#include "Legolas/Matrix/MatrixStructures/DRMatrix/VirtualDRMatrixInterface.hxx"

namespace Legolas{


  template <class DR_MATRIX_DEFINITION, class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE, class DR_INV, class DR_MULT>
  class ActualDRMatrixInterface{
  public:

    typedef ActualDRDataDriver<D_MATRIX_INTERFACE,R_MATRIX_INTERFACE> DataDriver;
    typedef typename DataDriver::DMatrixInterface DMatrixInterface;
    typedef typename DataDriver::RMatrixInterface RMatrixInterface;
  
    class Matrix : public ActualDRMatrix<DataDriver,Owner,DR_INV,DR_MULT>
    {
    private:
    
      typedef typename DR_MATRIX_DEFINITION::Data VirtualData;
      typedef ActualDRMatrix<DataDriver,Owner,DR_INV,DR_MULT> BaseClass;
      typedef typename BaseClass::Data Data;
      typedef typename VirtualDRMatrixInterface<DR_MATRIX_DEFINITION,D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT>::Matrix VirtualMatrix;
    public:
      typedef typename DataDriver::DMatrixInterface DMatrixInterface;
      typedef typename DataDriver::RMatrixInterface RMatrixInterface;

      Matrix( const Data & source ):BaseClass(source){
	MESSAGE("ActualDRMatrixInterface::Matrix Ctor");
      }

      //Actual Matrix Ctor from Virtual Container
      Matrix( const VirtualData & source):BaseClass(VirtualMatrix(source)){
	MESSAGE("ActualDRMatrixInterface:Matrix Ctor from Virtual Container ");
      }
    };

    typedef Matrix                                                  Copy;
    typedef ActualDRMatrix<DataDriver,NotOwner,DR_INV,DR_MULT>      View;
    typedef ActualDRMatrix<DataDriver,ConstNotOwner,DR_INV,DR_MULT> ConstView;
  };

}

#endif
      
      
      
      

      

