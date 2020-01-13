/**
 * project  DESCARTES
 *
 * @file     ActualDRMatrix.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ACTUALDRMATRIX_HXX__
#define __LEGOLAS_ACTUALDRMATRIX_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Matrix/StorageModeTraits/ConstRefTraits.hxx"


namespace Legolas{

  template <class ACTUAL_DR_DATA_DRIVER, template <class> class OWNERSHIP_POLICY,class DR_INV, class DR_MULT>
  class ActualDRMatrix:  public BaseMatrix< ActualDRMatrix<ACTUAL_DR_DATA_DRIVER,OWNERSHIP_POLICY,DR_INV,DR_MULT> >
  {
  public:
  
    typedef DR_MULT MatrixVectorProduct;
    typedef DR_INV  MatrixVectorInverse;

    typedef Actual StorageMode;

    typedef typename ConstRefTraits<ActualDRMatrix,StorageMode>::Type StoreType;

    typedef ACTUAL_DR_DATA_DRIVER                         ActualDRDataDriver;
    typedef typename ActualDRDataDriver::DMatrixInterface DMatrixInterface;
    typedef typename ActualDRDataDriver::RMatrixInterface RMatrixInterface;
    typedef typename ActualDRDataDriver::Data             Data;
    typedef OWNERSHIP_POLICY<Data>                        OPData;

    typedef typename ActualDRDataDriver::RealType      RealType;

    ActualDRMatrix( const Data & source ):data_(source){
      MESSAGE("ActualDRMatrix Ctor");
    }


    template <class INPUT_DR_MATRIX>
    ActualDRMatrix( const INPUT_DR_MATRIX & inputMatrix ):data_(Data()){
      ActualDRDataDriver::copy(inputMatrix,data_.getRef());
    }

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

    virtual inline ~ActualDRMatrix( void ){}


    //    static const int Level=LevelTraits<Shape>::Level;

//     void matrixVectorProduct(const AnyVector & aX, AnyVector & aY) const {
//       typedef Legolas::MultiVector<RealType,Level> V;
//       //      INFOS("ICI");
//       const V * XPtr=static_cast<const V*>(&aX);
//       V * YPtr=static_cast<V*>(&aY);

//       const Legolas::MultiVector<RealType,Level> & X(*XPtr);
//       Legolas::MultiVector<RealType,Level> & Y(*YPtr);

//       typedef typename Legolas::MultiVector<RealType,Level>::GetElement GetElement;
//       typedef typename MatrixVectorProduct::template Engine< EqualAssign<GetElement> > Operator;

//       Operator::apply(*this,X,Y);


//     }
    
      

    typename DMatrixInterface::View      D( void )       {return data_.getRef().dData() ; };
    typename DMatrixInterface::ConstView D( void ) const {return data_.getRef().dData() ; };
    typename RMatrixInterface::View      R( void )       {return data_.getRef().rData() ; };
    typename RMatrixInterface::ConstView R( void ) const {return data_.getRef().rData() ; };

    int getMaxIteration( void ) const{
      return ((*this).D()).getMaxIteration();
    }

    int & setMaxIteration( void ){
      return ((*this).D()).setMaxIteration();
    }

    RealType getEpsilon( void ) const{
      return ((*this).D()).getEpsilon();
    }

    RealType & setEpsilon( void ){
      return ((*this).D()).setEpsilon();
    }
  private:
    OPData data_;
  };

}

#endif
