/**
 * project  DESCARTES
 *
 * @file     MatrixBasicData.hxx
 *
 * @author Laurent PLAGNE
 * @date   October 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_MATRIXBASICDATA_HXX__
#define __LEGOLAS_MATRIXBASICDATA_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

namespace Legolas{

  template <class ELEMENT_DATA_DRIVER>
  class MatrixBasicData : public MatrixShape<ELEMENT_DATA_DRIVER::level+1>{
  public:
    static const int level=ELEMENT_DATA_DRIVER::level+1;
    typedef MatrixShape<level> Base;

    typedef typename ELEMENT_DATA_DRIVER::RealType RealType;
    typedef typename Base::Shape Shape;
    
    MatrixBasicData( void ):Base(),maxIteration_(0),epsilon_(0.0)
    {
      MESSAGE("MatrixBasicData Default Ctor");
    }

    MatrixBasicData(const Base & ms ):Base(ms),maxIteration_(0),epsilon_(0.0)
    {
      MESSAGE("MatrixBasicData Default Ctor");
    }

//     MatrixBasicData( int nrows, int ncols, int maxIter=100, RealType epsilon=1.e-6 ):nrows_(nrows),ncols_(ncols),maxIteration_(maxIter),epsilon_(epsilon)
//     {
//       MESSAGE("MatrixBasicData Default Ctor");
//     }
 //    inline int & nrows( void ) { return nrows_ ;}
//     inline int nrows( void ) const { return nrows_ ;}
//     inline int & ncols( void ) { return ncols_ ;}
//     inline int ncols( void ) const { return ncols_ ;}
    inline int & setMaxIteration( void ) { return maxIteration_ ;}
    inline int getMaxIteration( void ) const { return maxIteration_ ;}
    inline RealType & setEpsilon( void ) { return epsilon_ ;}
    inline const RealType & getEpsilon( void ) const { return epsilon_ ;}
//     inline ElementData & zero( void ) { return zero_ ;}
//     inline const ElementData & zero( void ) const { return zero_ ;}

 //    inline Shape & setRowShape( void ){ return rowShape_;}
//     inline const Shape & getRowShape( void ) const { return rowShape_;}

//     inline Shape & setColShape( void ){ return colShape_;}
//     inline const Shape & getColShape( void ) const { return colShape_;}

    inline bool outOfRectangularBounds(int i, int j) const{
      bool result=false;
      if ( (i<0) || (i>=this->nrows())){ 
	result=true;
	if (result) INFOS("OUT OF RECTANGULAR BOUNDS !! i="<<i<<" out of [0,"<<this->nrows()-1<<"]");
      }	
      if ( (j<0) || (j>=this->ncols())){
	result=true;      
	if (result) INFOS("OUT OF RECTANGULAR BOUNDS !! j="<<j<<" out of [0,"<<this->ncols()-1<<"]");
      }
      
      return result;
    }


  private:

    //   MatrixShape<ELEMENT_DATA_DRIVER::level+1> matrixShape_;

    //    int nrows_;
    //    int ncols_;
    int maxIteration_;
    RealType epsilon_;
    //    ElementData zero_;
    //    Shape rowShape_;
    //    Shape colShape_;
    
  };

}

#endif
  

  
