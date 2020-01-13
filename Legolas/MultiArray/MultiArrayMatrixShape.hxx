#pragma once
#include "Legolas/Matrix/VirtualMatrixShape.hxx"

namespace Legolas{

  template <class MAMS,class SMAMS,int LEVEL>
  struct MAMSTraits{
    static inline SMAMS * getSubMatrixShapePtr(const MAMS & mams, int i , int j){
      return new SMAMS(mams.rowShape_[i],mams.rowShape_[j]);
    }
    
    // static inline SMAMS getSubMatrixShape(const MAMS & mams, int i , int j){
    //   return SMAMS(mams.rowShape_[i],mams.rowShape_[j]);
    // }

    
  };

  template <class MAMS,class SMAMS>
  struct MAMSTraits<MAMS,SMAMS,1>{
    static inline MAMS * getSubMatrixShapePtr(const MAMS & mams, int i , int j){
      INFOS("ERROR: No sub Matrix Shape!!");
      throw std::runtime_error("ERROR: No sub Matrix Shape!!");
      return new MAMS(mams);
    }
  };

  
  template <class MULTI_ARRAY_VECTOR_SHAPE>
  struct MultiArrayMatrixShape:public VirtualMatrixShape{

    typedef MULTI_ARRAY_VECTOR_SHAPE Shape;
    static const int level_=Shape::level_;
    static const int multiArrayLevel_=Shape::multiArrayLevel_;
    
    typedef MultiArrayMatrixShape<typename MULTI_ARRAY_VECTOR_SHAPE::SubShape> SMS;
    
    Shape rowShape_;
    Shape colShape_;

    MultiArrayMatrixShape(const Shape & rowShape, const Shape & colShape):rowShape_(rowShape),
									  colShape_(colShape){
    }

    int nrows() const {return rowShape_.size();}
    int ncols() const {return colShape_.size();}

    int getLevel( void ) const { return level_;}
    
    
    VirtualMatrixShape * getSubMatrixShapePtr(int i , int j) const {
      //      typedef MultiArrayMatrixShape<typename MULTI_ARRAY_VECTOR_SHAPE::SubShape> SMS;
      return MAMSTraits<MultiArrayMatrixShape,SMS,Shape::multiArrayLevel_>::getSubMatrixShapePtr(*this,i,j);
      //			typedef MultiArrayMatrixShape<typename MULTI_ARRAY_VECTOR_SHAPE::SubShape> SMS;
      //      return new SMS(rowShape_[i],colShape_[j]);
    }

    inline SMS getSubMatrixShape(int i , int j) const {
      return SMS(rowShape_[i],colShape_[j]);
    }
    
    
    VirtualMatrixShape * clone( void ) const {
      return new MultiArrayMatrixShape(*this);
    }
    
    VirtualMatrixShape * newVirtualMatrixShape(const VirtualVectorShape & l,  const VirtualVectorShape & r) const{
      const Shape & left=static_cast<const Shape &>(l);
      const Shape & right=static_cast<const Shape &>(r);
      return new MultiArrayMatrixShape(left,right);
    }
															  
    const VirtualVectorShape & getRowShape( void ) const { return rowShape_;}
    const VirtualVectorShape & getColShape( void ) const { return colShape_;}
    
  };

}
    


    

      
      
    
