#ifndef __MATRIXSHAPE_HXX__
#define __MATRIXSHAPE_HXX__

#include "Legolas/Vector/Shape/ShapeTraits.hxx"
#include "Legolas/Matrix/VirtualMatrixShape.hxx"

namespace Legolas{

  // class VirtualMatrixShape{
  // public:
  //   virtual int nrows() const =0 ; 
  //   virtual int ncols() const =0 ;
    
  //   virtual VirtualMatrixShape * getSubMatrixShapePtr(int i , int j) const =0;
  //   virtual VirtualMatrixShape * clone( void ) const =0;
  //   virtual VirtualMatrixShape * newVirtualMatrixShape(const VirtualVectorShape & l,  const VirtualVectorShape & r) const =0;
  //   virtual const VirtualVectorShape & getRowShape( void ) const = 0 ;
  //   virtual const VirtualVectorShape & getColShape( void ) const = 0 ;
  
  //   virtual ~VirtualMatrixShape( void ) {}; 

  //   virtual int getLevel( void ) const =0;

  //   // virtual VirtualMatrixShape * getProductMatrixShape(const VirtualMatrixShape & right) const = 0 ;
      
  // };

  template <class MATRIX_SHAPE, class SUB_MATRIX_SHAPE, int LEVEL>
  class GetSubMatrixShapeTraits{
  public:
    typedef SUB_MATRIX_SHAPE ReturnType;


    static inline SUB_MATRIX_SHAPE * apply(const MATRIX_SHAPE & ms, int i , int j){
      return new SUB_MATRIX_SHAPE(ms.getRowShape()[i],ms.getColShape()[j]);
    }
  };

  template <class MATRIX_SHAPE, class SUB_MATRIX_SHAPE>
  class GetSubMatrixShapeTraits<MATRIX_SHAPE,SUB_MATRIX_SHAPE,1>{
  public:
    
    typedef MATRIX_SHAPE ReturnType;

    static inline MATRIX_SHAPE * apply(const MATRIX_SHAPE & ms, int i , int j){
      INFOS("ERROR: No sub Matrix Shape!!");
      return new MATRIX_SHAPE(ms);
    }
  };


  template <int LEVEL>
  class MatrixShape:public VirtualMatrixShape{
  public:
    static const int level=LEVEL;

    typedef typename ShapeTraits<LEVEL>::Type Shape;

    ~MatrixShape( void ){
      //      INFOS("ICI");
    }

    MatrixShape( void ):rowShape_(),
			colShape_(){
    }    


    MatrixShape(const MatrixShape & source ):rowShape_(source.rowShape_),
					     colShape_(source.colShape_)
    {
    }    


    MatrixShape(const Shape & rowShape, 
		const Shape & colShape):rowShape_(rowShape),
					colShape_(colShape){
    }
    
    inline const Shape & getRowShape( void ) const { return rowShape_ ;}
    inline const Shape & getColShape( void ) const { return colShape_ ;}

    inline Shape & setRowShape( void ) { return rowShape_ ;}
    inline Shape & setColShape( void ) { return colShape_ ;}
    
    //    inline int nrows( void ) const { return Legolas::getSize(rowShape_) ; }
    //    inline int ncols( void ) const { return Legolas::getSize(colShape_) ; }

    inline int nrows( void ) const { return rowShape_.size() ; }
    inline int ncols( void ) const { return colShape_.size() ; }


    typedef MatrixShape<LEVEL>   MS;
    typedef MatrixShape<LEVEL-1> SMS;
    typedef typename GetSubMatrixShapeTraits<MS,SMS,LEVEL>::ReturnType GetSMS;

    inline SMS getSubMatrixShape(int i , int j) const {
      return SMS(rowShape_[i],colShape_[j]);
      //      typedef MatrixShape<LEVEL> MS;
      //      typedef MatrixShape<LEVEL-1> SMS;
      //      return GetSubMatrixShapeTraits<MS,SMS,LEVEL>::apply(*this,i,j);
      //      return MatrixShape<LEVEL-1>(rowShape_[i],colShape_[j]);
    }

    inline VirtualMatrixShape * getSubMatrixShapePtr(int i , int j) const {
      return GetSubMatrixShapeTraits<MS,SMS,LEVEL>::apply(*this,i,j);
    }

    // inline size_t getSubFlatRowSize( int i ) const{
    //   return Legolas::getSubFlatSize(rowShape_,i);
    // }
    
    // inline size_t getSubFlatColSize( int j ) const{
    //   return Legolas::getSubFlatSize(colShape_,j);
    // }

    // inline size_t getFlatRowSize( void ) const{
    //   return Legolas::getFlatSize(rowShape_);
    // }

    // inline size_t getFlatColSize( void ) const{
    //   return Legolas::getFlatSize(colShape_);
    // }

    inline int getLevel( void ) const { return LEVEL ;}

    
    inline VirtualMatrixShape * clone( void ) const {
      return new MatrixShape(*this);
    }

    inline VirtualMatrixShape * newVirtualMatrixShape(const VirtualVectorShape & l,  const VirtualVectorShape & r ) const {
      const Shape & left=static_cast<const Shape &>(l);
      const Shape & right=static_cast<const Shape &>(r);
      return new MatrixShape(left,right);
    }

    // virtual VirtualMatrixShape * getProductMatrixShape(const VirtualMatrixShape & right) const {
    //   const MatrixShape & r=static_cast<const MatrixShape &>(right);  
    //   assert((this->getRowShape()==r.getRowShape()));
    //   return new MatrixShape(this->getRowShape(),r.getColShape());
    // }
    
    inline bool outOfRectangularBounds(int i, int j) const{
      bool result=false;
      if ( (i<0) || (i>=this->nrows())){ 
	result=true;
	if (result) INFOS("OUT OF RECTANGULAR BOUNDS !! i="<<i<<" out of [0,"<<this->nrows()<<"]");
      }	
      if ( (j<0) || (j>=this->ncols())){
	result=true;      
	if (result) INFOS("OUT OF RECTANGULAR BOUNDS !! j="<<j<<" out of [0,"<<this->ncols()<<"]");
      }
      
      return result;
    }


  private:
    Shape rowShape_;
    Shape colShape_;
  };


  template <int LEVEL> 
  std::ostream& operator<< ( std::ostream& os, 
			     const MatrixShape<LEVEL> & ms )
  {
    os <<  std::endl;
    os << "ms.getRowShape()="<<ms.getRowShape()<< std::endl;
    os << "ms.getColShape()="<<ms.getColShape()<< std::endl;

    return os;
  }

}

#endif
      
      
    
    
    
  
  
