/*
 * project  DESCARTES
 *
 * @file     TinyBandedSymmetricMatrixContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TINYBANDEDSYMMETRICMATRIXCONTAINER_HXX__
#define __LEGOLAS_TINYBANDEDSYMMETRICMATRIXCONTAINER_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{
  
  class TinyBandedSymmetricMatrixContainer {
  public:
    
    template < class MATRIX_OPTIONS, class ELEMENT_DATA_DRIVER>
    class Engine : public MatrixBasicEngine<ELEMENT_DATA_DRIVER>{
    public:
      
      typedef typename ELEMENT_DATA_DRIVER::Data                      ElementData;
      typedef typename ELEMENT_DATA_DRIVER::RealType                  RealType;
      typedef typename MATRIX_OPTIONS::VectorContainer                VectorContainer;
      typedef typename VectorContainer::template Engine<ElementData>  VectorInterface;
      typedef typename VectorInterface::Vector                        Vector;
      
      //**************************************************************
      class Data  : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
      private:
	inline int dataSize( void ) const{
	  return this->nrows()*hbw_+this->nrows()-hbw_;
	}
	inline int getDataIndex(int rowIndex, int colIndex) const{
	  //    return rowIndex*hbw_+colIndex+hbw_;
	  return rowIndex*hbw_+colIndex;
	}
      public:
	


	inline void resize( int nrows, int hbw ){
	  hbw_=hbw;
	  VectorInterface::resize(this->dataSize(),data_);
	}
	
	inline void resize( int nrows, int hbw, RealType value ){
	  this->resize(nrows,hbw);
	  data_=value;
	}
  
	Data( void ):data_(),hbw_(0){}
  
	Data(int nrows , int hbw, RealType value):data_(),hbw_(0){
	  this->resize(nrows,hbw,value);
	}
  	Data(int nrows, int hbw ):data_(),hbw_(0){ 
	  this->resize(nrows,hbw);
	}
	
	
	inline const RealType & lowerBandedGetElement(int rowIndex, int colIndex) const{
	  return data_[getDataIndex(rowIndex,colIndex)];
	}
  
	inline RealType & lowerBandedGetElement(int rowIndex, int colIndex){
	  return data_[getDataIndex(rowIndex,colIndex)];
	}
  
	//************ Other Accessor ****************

	inline int getHalfBandwidth( void ) const { return hbw_;}

	//************ Misc ****************  

	inline RealType memoryUsageInMB( void ) const {
	  RealType elementNumber=RealType(this->dataSize());
	  RealType elementSizeInByte=RealType(sizeof(RealType));
	  RealType BToMB=1.0/RealType(1024*1024);
	  return elementNumber*elementSizeInByte*BToMB;
	}
  
      private:
	Vector data_;
	int hbw_;
      };
      //**************************************************************
      
      template <int LEVEL>
      static inline void resize(const MatrixShape<LEVEL> & shape, int hbw, Data & A){
	A.setColShape()=shape.getColShape();
	A.setRowShape()=shape.getRowShape();
	A.resize(shape.nrows(),hbw);
	//	VectorInterface::resize(shape.nrows(),A.matrix());
      }

      
      //      static inline void resize(int nrows, int hbw, Data & data){data.resize(nrows,hbw);}
      
      static inline ElementData & lowerBandedGetElement(int i,int j, Data & data){
	return data.lowerBandedGetElement(i,j);
      }
      
      static inline const ElementData & lowerBandedGetElement(int i,int j, const Data & data){
	return data.lowerBandedGetElement(i,j);
      }

      static inline int getHalfBandwidth(const Data & data){ return data.getHalfBandwidth();}
      
    };
    
  };
}

#endif
  

  
