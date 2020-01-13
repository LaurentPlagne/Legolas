#pragma once

#include "Legolas/Array/BaseArray.hxx"

namespace Legolas{


  struct AddArray{
    template <class LEFT, class RIGHT>
    static inline
    const auto apply(const LEFT & left, const RIGHT & right){
      //auto apply(LEFT left, RIGHT right){
      return left+right;
    }
  };

  struct MinusArray{
    template <class LEFT, class RIGHT>
    static inline
    auto apply(const LEFT & left, const RIGHT & right) -> decltype(left-right){
      //auto apply(LEFT left, RIGHT right){
      return left-right;
    }
  };

  struct MultArray{
    template <class LEFT, class RIGHT>
    static inline
    auto apply(const LEFT & left, const RIGHT & right)  -> decltype(left*right){
      //auto apply(LEFT left, RIGHT right){
      return left*right;
    }
  };


  template <class LEFT, class BINARY_OPERATOR , class RIGHT>
  struct ArrayExpression: public BaseArray< ArrayExpression<LEFT,BINARY_OPERATOR,RIGHT> >{

    typedef ArrayExpression StoreType;
    typedef typename RIGHT::GetElement GetElement;
    typedef typename RIGHT::ConstGetElement ConstGetElement;
    typedef typename RIGHT::RealType RealType;
    typedef typename RIGHT::PackedRealType PackedRealType;
    typedef typename RIGHT::PackedDoubleType PackedDoubleType;

    static const int level=RIGHT::level;
    static const int packLevel=RIGHT::packLevel;
    static const int packSize=RIGHT::packSize;

    size_t flatSize( void ) const { return right_.flatSize();}


    ArrayExpression(const BaseArray<LEFT> & left, const BaseArray<RIGHT> & right):left_(left.getArrayRef()),
										  right_(right.getArrayRef())
    {
      //      assert(RIGHT::packLevel==LEFT::packLevel);
      assert(left_.shape()==right_.shape());
	     //      assert(left_.size()==right_.size());
    }

    inline auto operator[]( int i ) const {
      return BINARY_OPERATOR::apply(left_[i],right_[i]);
    }

    size_t size( void ) const {
      return left_.size();
    }

    const typename LEFT::StoreType left_;
    const typename RIGHT::StoreType right_;

    typedef typename RIGHT::Shape Shape;
    //    typedef typename RIGHT::ConstShapeRef ConstShapeRef;

    const Shape & shape( void ) const { return right_.shape();}

    typedef typename LEFT::PackedArrayView LPacked;
    typedef typename RIGHT::PackedArrayView RPacked;
    typedef ArrayExpression<LPacked,BINARY_OPERATOR,RPacked> PackedArrayView;

    PackedArrayView getPackedView( void ) const {
      return PackedArrayView(left_.getPackedView(),right_.getPackedView());
    }


    typedef typename LEFT::ArrayView LArray;
    typedef typename RIGHT::ArrayView RArray;
    typedef ArrayExpression<LArray,BINARY_OPERATOR,RArray> ArrayView;

    ArrayView getArrayView( void ) const {
      return ArrayView(left_.getArrayView(),right_.getArrayView());
    }





    typedef typename LEFT::FlatPackedArrayView LFlat;
    typedef typename RIGHT::FlatPackedArrayView RFlat;
    typedef ArrayExpression<LFlat,BINARY_OPERATOR,RFlat> FlatPackedArrayView;

    FlatPackedArrayView getFlatPackedView( void ) const {
      return FlatPackedArrayView(left_.getFlatPackedView(),right_.getFlatPackedView());
    }

    typedef typename LEFT::EigenView LEigen;
    typedef typename RIGHT::EigenView REigen;
    typedef ArrayExpression<LEigen,BINARY_OPERATOR,REigen> EigenView;

    //    inline auto getEigenView( void ) const ->decltype(BINARY_OPERATOR::apply(left_.getEigenView(),right_.getEigenView())) {
    inline auto getEigenView( void ) const {
      return BINARY_OPERATOR::apply(left_.getEigenView(),right_.getEigenView());
      //      return left_.getEigenView()+right_.getEigenView();
    }

    inline auto getEigenView(size_t begin, size_t chunkSize) const {
      return BINARY_OPERATOR::apply(left_.getEigenView(begin,chunkSize),right_.getEigenView(begin,chunkSize));
      //      return left_.getEigenView(begin,chunkSize)+right_.getEigenView(begin,chunkSize);
    }




  };

  template <class ARRAY>
  struct ScaledArray: public BaseArray< ScaledArray<ARRAY> >{

    typedef ScaledArray StoreType;
    typedef typename ARRAY::GetElement GetElement;
    typedef typename ARRAY::ConstGetElement ConstGetElement;
    typedef typename ARRAY::RealType RealType;
    typedef typename ARRAY::PackedRealType PackedRealType;
    typedef typename ARRAY::PackedDoubleType PackedDoubleType;
    static const int level=ARRAY::level;
    static const int packLevel=ARRAY::packLevel;
    static const int packSize=ARRAY::packSize;

    const ARRAY array_;
    const RealType coef_;


    ScaledArray(const BaseArray<ARRAY> & array, RealType coef):array_(array.getArrayRef()),
								     coef_(coef)
    {
    }

    inline auto operator[]( int i ) const ->decltype(coef_*array_[i]) {
      //inline auto operator[]( int i ) const {
      //      INFOS("i="<<i);
      return coef_*array_[i];
    }

    inline int size( void ) const {
      return array_.size();
    }

    typedef typename ARRAY::Shape Shape;
    //    typedef typename ARRAY::ConstShapeRef ConstShapeRef;

    const Shape & shape( void ) const { return array_.shape();}


    typedef typename ARRAY::PackedArrayView APacked;
    typedef ScaledArray<APacked> PackedArrayView;

    PackedArrayView getPackedView( void ) const {
      typename APacked::RealType fc(coef_);
      return PackedArrayView(array_.getPackedView(),fc);
    }

    typedef typename ARRAY::ArrayView AArray;
    typedef ScaledArray<AArray> ArrayView;

    ArrayView getArrayView( void ) const {
      return ArrayView(array_.getArrayView(),coef_);
    }

    typedef typename ARRAY::FlatPackedArrayView AFlat;
    typedef ScaledArray<AFlat> FlatPackedArrayView;

    FlatPackedArrayView getFlatPackedView( void ) const {
      typename AFlat::RealType fc(coef_);
      return FlatPackedArrayView(array_.getFlatPackedView(),fc);
    }

    typedef typename ARRAY::EigenView AEigen;
    typedef ScaledArray<AEigen> EigenView;

    inline auto getEigenView( void ) const{
      return array_.getEigenView()*coef_;
      //      typename AFlat::RealType fc(coef_);
      //      return EigenView(array_.getEigenView(),fc);
    }

    inline auto getEigenView(size_t begin, size_t chunkSize) const {
      return array_.getEigenView(begin,chunkSize)*coef_;
    }

  };


  template <class LEFT, class RIGHT>
  ArrayExpression<LEFT,AddArray,RIGHT> operator+ (const BaseArray<LEFT> & left, const BaseArray<RIGHT> & right){
    return ArrayExpression<LEFT,AddArray,RIGHT>(left,right);
  }
  template <class LEFT, class RIGHT>
  ArrayExpression<LEFT,MinusArray,RIGHT> operator- (const BaseArray<LEFT> & left, const BaseArray<RIGHT> & right){
    return ArrayExpression<LEFT,MinusArray,RIGHT>(left,right);
  }

  template <class LEFT, class RIGHT>
  ArrayExpression<LEFT,MultArray,RIGHT> operator* (const BaseArray<LEFT> & left, const BaseArray<RIGHT> & right){
    return ArrayExpression<LEFT,MultArray,RIGHT>(left,right);
  }

  template <class ARRAY>
  ScaledArray<ARRAY> operator*(double coef, const BaseArray<ARRAY> & right){
    return ScaledArray<ARRAY>(right,typename ARRAY::RealType(coef));
  }

  template <class ARRAY>
  ScaledArray<ARRAY> operator*(const BaseArray<ARRAY> & right, double coef){
    return ScaledArray<ARRAY>(right,typename ARRAY::RealType(coef));
  }

  template <class ARRAY>
  struct ParallelArray{
    const ARRAY & array_;
    ParallelArray(const  BaseArray<ARRAY> & a):array_(a.getArrayRef()){
    }
    const ARRAY & getArray( void ) const { return array_;}
    const ARRAY & getArrayRef( void ) const { return array_;}
  };

  enum EvalMode{par};

  template <class ARRAY>
  ParallelArray<ARRAY> operator||(const BaseArray<ARRAY> & left, EvalMode mode){
    return ParallelArray<ARRAY>(left);
  }



};
