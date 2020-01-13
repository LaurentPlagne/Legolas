#pragma once

#include <type_traits>
#include "Legolas/EigenInterface.hxx"
#include <Eigen/Core>
#include "Legolas/Array/Block/BaseBlock.hxx"
#include "stringConvert.hxx"
#include "Legolas/Array/Block/BlockExpression.hxx"
#include <array>

namespace Legolas{

  template <int SIZE, int... SUB_SIZES>
  struct BlockShape{
    using Element=BlockShape<SUB_SIZES...>;

    constexpr static int elementFlatSize_=Element::flatSize_;
    constexpr static int size_=SIZE;
    constexpr static int flatSize_=size_*elementFlatSize_;


    template <class SCALAR_TYPE>
    constexpr static SCALAR_TYPE * getElement(int i,SCALAR_TYPE * data){
      return data+i*elementFlatSize_;
    }

    template <template <class,class> class BLOCK_VIEW, class SCALAR_TYPE>
    using BlockElement=BLOCK_VIEW<SCALAR_TYPE,Element>;

    constexpr static const bool is1D=false;
  };

  template <int SIZE>
  struct BlockShape<SIZE>{
    using Element=void;
    constexpr static int size_=SIZE;
    constexpr static int flatSize_=size_;
    constexpr static int elementFlatSize_=1;

    template <template <class,class> class BLOCK_VIEW, class SCALAR_TYPE>
    using BlockElement=SCALAR_TYPE &;


    template <class SCALAR_TYPE>
    constexpr static SCALAR_TYPE & getElement(int i,SCALAR_TYPE * data){
      return *(data+i);
    }
    constexpr static const bool is1D=true;
  };




  template <class SCALAR_TYPE, class BLOCK_SHAPE>
  struct BlockView: public BaseBlock<BlockView<SCALAR_TYPE,BLOCK_SHAPE>>{

    using ScalarType=SCALAR_TYPE;
    using Shape=BLOCK_SHAPE;
    using ElementShape=typename Shape::Element;

    //LP: If I remove the Legolas namespace: compilation fails... odd
    using Element=typename Shape::template BlockElement<Legolas::BlockView,ScalarType>;

    ScalarType * data_;
    BlockView(ScalarType * data):data_(data){}

    constexpr int size() const {return Shape::size_;}

    Element operator[](int i){return Shape::getElement(i,data_);}
    const Element operator[](int i) const {return Shape::getElement(i,data_);}

    using EigenBlock=Eigen::Array<ScalarType,Shape::flatSize_, 1>;
    using EigenBlockMap=typename Legolas::EigenBlockMap<EigenBlock>::Map;
    using ConstEigenBlockMap=typename Legolas::EigenBlockMap<const EigenBlock>::Map;


    constexpr EigenBlockMap getEigenView( void ){ return EigenBlockMap(data_);}
    constexpr ConstEigenBlockMap getEigenView( void ) const { return ConstEigenBlockMap(data_);}

    template <class DERIVED>
    BlockView & operator=(const BaseBlock<DERIVED> & right){
      (*this).getEigenView()=right.getBlockRef().getEigenView();
      return (*this);
    }
    template <class DERIVED>
    BlockView & operator+=(const BaseBlock<DERIVED> & right){
      (*this).getEigenView()+=right.getBlockRef().getEigenView();
      return (*this);
    }
    template <class DERIVED>
    BlockView & operator-=(const BaseBlock<DERIVED> & right){
      (*this).getEigenView()-=right.getBlockRef().getEigenView();
      return (*this);
    }

    BlockView & operator=(ScalarType value){
      (*this).getEigenView()=value;
      return (*this);
    }
    BlockView & operator*=(ScalarType value){
      (*this).getEigenView()*=value;
      return (*this);
    }

    std::ostream & display(std::ostream& os) const{
      os<<"[";
      for (int i=0 ; i<this->size();i++){
        os <<(*this)[i];
        if (i!=(this->size()-1)) os<<",";
      }
      os<<"]";
      return os;
    }

  };

  template <class SCALAR_TYPE, class BLOCK_SHAPE>
  std::ostream& operator << (std::ostream& os, const BlockView<SCALAR_TYPE,BLOCK_SHAPE> & b){
    return b.display(os);
    //    return os;
  }

  template <class SCALAR_TYPE, class BLOCK_SHAPE>
  struct Block: public BaseBlock<Block<SCALAR_TYPE,BLOCK_SHAPE>>{

    using ScalarType=SCALAR_TYPE;
    using Shape=BLOCK_SHAPE;
    using ElementShape=typename Shape::Element;

    //LP: If I remove the Legolas namespace: compilation fails... odd
    using Element=typename Shape::template BlockElement<Legolas::BlockView,ScalarType>;

    alignas(32) std::array<ScalarType,Shape::flatSize_> data_;
    Block( void ):data_(){}

    constexpr int size() const {return Shape::size_;}

    constexpr ScalarType * ptr() {return &data_[0];}
    constexpr const ScalarType * ptr() const  {return &data_[0];}

    Element operator[](int i){return Shape::getElement(i,ptr());}
    const Element operator[](int i) const {return const_cast<ScalarType *>(Shape::getElement(i,ptr()));}

    using EigenBlock=Eigen::Array<ScalarType,Shape::flatSize_, 1>;
    using EigenBlockMap=typename Legolas::EigenBlockMap<EigenBlock>::Map;
    using ConstEigenBlockMap=typename Legolas::EigenBlockMap<const EigenBlock>::Map;


    constexpr EigenBlockMap getEigenView( void ){ return EigenBlockMap(ptr());}
    constexpr ConstEigenBlockMap getEigenView( void ) const { return ConstEigenBlockMap(ptr());}

    template <class DERIVED>
    Block & operator=(const BaseBlock<DERIVED> & right){
      (*this).getEigenView()=right.getBlockRef().getEigenView();
      return (*this);
    }
    template <class DERIVED>
    Block & operator+=(const BaseBlock<DERIVED> & right){
      (*this).getEigenView()+=right.getBlockRef().getEigenView();
      return (*this);
    }
    template <class DERIVED>
    Block & operator-=(const BaseBlock<DERIVED> & right){
      (*this).getEigenView()-=right.getBlockRef().getEigenView();
      return (*this);
    }

    Block & operator=(ScalarType value){
      (*this).getEigenView()=value;
      return (*this);
    }
    Block & operator*=(ScalarType value){
      (*this).getEigenView()*=value;
      return (*this);
    }

    std::ostream & display(std::ostream& os) const{
      os<<"[";
      for (int i=0 ; i<this->size();i++){
        os <<(*this)[i];
        if (i!=(this->size()-1)) os<<",";
      }
      os<<"]";
      return os;
    }

  };

  template <class SCALAR_TYPE, class BLOCK_SHAPE>
  std::ostream& operator << (std::ostream& os, const Block<SCALAR_TYPE,BLOCK_SHAPE> & b){
    return b.display(os);
    //    return os;
  }



}
