/**
 * project  DESCARTES
 *
 * @file     VirtualDRMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_VIRTUALDRMATRIXINTERFACE_HXX__
#define __LEGOLAS_VIRTUALDRMATRIXINTERFACE_HXX__

#include "Legolas/Vector/BaseMatrix.hxx"

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/StorageModeTraits/ConstRefTraits.hxx"

namespace Legolas{

  template <class DR_MATRIX_DEFINITION, class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE, class DR_INV, class DR_MULT>
  class VirtualDRMatrix: public BaseMatrix< VirtualDRMatrix<DR_MATRIX_DEFINITION,D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT> >
  {
  public:

    typedef DR_MULT MatrixVectorProduct;
    typedef DR_INV  MatrixVectorInverse;
  
    typedef Virtual StorageMode;

    typedef typename ConstRefTraits<VirtualDRMatrix,StorageMode>::Type StoreType;
   

    typedef typename DR_MATRIX_DEFINITION::Data Data;
    typedef D_MATRIX_INTERFACE DMatrixInterface;
    typedef R_MATRIX_INTERFACE RMatrixInterface;
  
    VirtualDRMatrix( const Data & source):_data(source){
      MESSAGE("VirtualDRMatrixInterface::MATRIX::Data Ctor");
    }
  
    VirtualDRMatrix( const VirtualDRMatrix & source):_data(source.data()){
      MESSAGE("VirtualDRMatrixInterface::MATRIX::Data Copy Ctor");
    }
  
    Data & data( void ) { return _data ; } ;
    const Data & data( void ) const { return _data ; } ;

    std::string getStructureName( void ) const {
      typedef typename DMatrixInterface::DataDriver DDataDriver;    
      typedef typename RMatrixInterface::DataDriver RDataDriver;    
      std::string result="DRMatrix (D=";
      result+=DDataDriver::name();
      result+=" , R=";
      result+=RDataDriver::name();
      result+=")";
      return result;
    }
    
    virtual void sparseRecursiveDisplayLatexMethod(std::ofstream & outfile, int flag=0) const {
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }
    virtual void sparseRecursiveDisplayHTMLMethod(const std::string & filename, std::ofstream & outfile, const std::string * parentNamePtr=0, int flag=0) const{
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }
    virtual void matrixVectorProduct(const AnyVector & X, AnyVector & Y) const {
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    };

    virtual inline ~VirtualDRMatrix( void ){}

  
    typename DMatrixInterface::Copy D( void ) const { return _data.dData() ;};
    typename RMatrixInterface::Copy R( void ) const { return _data.rData() ;};

    int getMaxIteration( void ) const{
      return ((*this).D()).getMaxIteration();
    }

    typedef typename DMatrixInterface::Matrix::RealType RealType;

    RealType getEpsilon( void ) const{
      return ((*this).D()).getEpsilon();
    }

  private:
  
    Data _data;
  };


  template <class DR_MATRIX_DEFINITION, class D_MATRIX_INTERFACE, class R_MATRIX_INTERFACE, class DR_INV, class DR_MULT>
  class VirtualDRMatrixInterface{
  public:
  
    typedef VirtualDRMatrix<DR_MATRIX_DEFINITION,D_MATRIX_INTERFACE,R_MATRIX_INTERFACE,DR_INV,DR_MULT> Matrix;
    typedef D_MATRIX_INTERFACE DMatrixInterface;
    typedef R_MATRIX_INTERFACE RMatrixInterface;
    typedef Matrix   Copy;
    typedef Matrix   View;
    typedef Matrix   ConstView;

  };

}

#endif    

    

      
      
  
