#ifndef __LEGOLAS_SINGLEELEMENTMATRIXDATA_HXX__
#define __LEGOLAS_SINGLEELEMENTMATRIXDATA_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  template <class ELEMENT_DATA_DRIVER>
  class SingleElementMatrixData : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
  public:
  
    typedef typename ELEMENT_DATA_DRIVER::RealType     RealType;
    typedef typename ELEMENT_DATA_DRIVER::Data         ElementData;


    SingleElementMatrixData( void ):rowIndex_(0),
				    colIndex_(0),
				    value_()
    {
      MESSAGE("Single::Data Default Ctor");
    }

    SingleElementMatrixData(int nrows, 
			    int ncols, 
			    int rowIndex, 
			    int colIndex, 
			    const ElementData & value):MatrixBasicData<ELEMENT_DATA_DRIVER>(nrows,ncols),
						       rowIndex_(rowIndex),
						       colIndex_(colIndex),
						       value_(value)
    {
      MESSAGE("Single::Data Normal Ctor");
    }
  
    inline const int & rowIndex( void ) const { return rowIndex_ ;}
    inline int & rowIndex( void ) { return rowIndex_ ;}      
    inline const int & colIndex( void ) const { return colIndex_ ;}
    inline int & colIndex( void ) { return colIndex_ ;}      
    inline const ElementData & value( void ) const { return value_ ;}
    inline ElementData & value( void ) { return value_ ;}      
  
  private:

    int rowIndex_;
    int colIndex_;
    ElementData value_ ;
  
  };

}

#endif
