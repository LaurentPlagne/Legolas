#pragma once

#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include "Legolas/Allocator.hxx"
#include "Legolas/Array/ArrayShape.hxx"
#include "stringConvert.hxx"
#include "Legolas/Array/BaseArray.hxx"
#include "Legolas/Array/ForEach.hxx"
#include "Legolas/Array/Expression.hxx"
#include "Legolas/Array/Reductions.hxx"
#include "Legolas/StaticArray/StaticArray.hxx"
#include "Legolas/Array/NativeSimd.hxx"

#if defined(__GNUC__) || defined(__clang__)
#  define LEGOLAS_ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#  define LEGOLAS_ALWAYS_INLINE __forceinline
#else
#  define LEGOLAS_ALWAYS_INLINE inline
#endif

namespace Legolas{

  template < class ARRAY, int LEVEL>
  struct ArrayRecursiveTraits{
    typedef typename ARRAY::Element GetElement;
    typedef const typename ARRAY::Element ConstGetElement;
    static inline GetElement getElement(ARRAY & array, int i){
      return GetElement(array.shape_.subShape_,array.dataPtr_+array.shape_.computeShift(i));
    }
    static inline const GetElement getElement(const ARRAY & array, int i){
      return GetElement(array.shape_.subShape_,array.dataPtr_+array.shape_.computeShift(i));
    }


    static inline void display(const ARRAY & vec, std::ostream& os, const std::string & prefix){
      //      os << " -- realDataPtr_=";
      //      os << vec.realDataPtr() << std::endl;
      os << std::endl;
      //      INFOS("vec.size()="<<vec.size());

      for (int i=0 ; i < vec.size() ; i++){
        std::string indexString(prefix);
        indexString+="[\t";
        indexString+=stringConvert(i);
        indexString+="\t]";
        vec[i].display(os,indexString);
      }

      return ;
    }

    // template <class INIT_FUNCTOR, typename... Args>
    // static inline void initialize(ARRAY & a, const INIT_FUNCTOR & initFunctor,const Args... rest){
    //   //      std::cout << "initiliaze Level " << LEVEL << std::endl;
    //   for(int i=0 ; i< a.size() ; i++ ) {
    // 	//	std::cout << "i="<< i<< std::endl;
    // 	a[i].initialize(initFunctor,rest...,i);
    // 	//	a[i]=initFunctor(rest,i);
    //   }
    // }


    template <class INIT_FUNCTOR>
    static inline void initialize(ARRAY & a, const INIT_FUNCTOR & initFunctor){
      //      std::cout << "initiliaze Level " << LEVEL << std::endl;
      for(int i=0 ; i< a.size() ; i++ ) {
        //	std::cout << "i="<< i<< std::endl;
        a[i].initialize(initFunctor,i);
        //	a[i]=initFunctor(rest,i);
      }
    }



    static inline void fillNoPad(ARRAY & a, typename ARRAY::RealType value){
      //      INFOS("LEVEL="<<LEVEL<<" a.size()="<<a.size());
      for(int i=0 ; i< a.size() ; i++ ) {
        a[i].fillNoPad(value);
      }
    }




  };

  template < class ARRAY>
  struct ArrayRecursiveTraits<ARRAY,1>{
    typedef typename ARRAY::RealType & GetElement;
    typedef const typename ARRAY::RealType & ConstGetElement;

    static const size_t stride_=ARRAY::Shape::stride_;

    static inline GetElement getElement(ARRAY & array, int i){
      return array.dataPtr_[stride_*i];
      //      return array.dataPtr_[array.shape_.computeShift(i)];
    }
    static inline ConstGetElement getElement(const ARRAY & array, int i){
      return array.dataPtr_[stride_*i];
      //      return array.dataPtr_[array.shape_.computeShift(i)];
    }

    static inline void display(const ARRAY & vec, std::ostream& os, const std::string & prefix){
      os << prefix << "={ " ;


      for(int k=0 ; k< vec.size() ; k++ ) {
        os  << vec[k] << ' ';
      }
      os << "}" << std::endl;

      return ;
    }

    template <class INIT_FUNCTOR, typename... Args>
    static inline void initialize(ARRAY & a, const INIT_FUNCTOR & initFunctor,const Args... rest){
      //      std:: cout << "initiliaze Level 1 " << std::endl;
      for(int i=0 ; i< a.size() ; i++ ) {
        //	std::cout << "i="<< i<< std::endl;
        a[i]=initFunctor(rest...,i);
      }
    }


    template <class INIT_FUNCTOR>
    static inline void initialize(ARRAY & a, const INIT_FUNCTOR & initFunctor){
      //      std:: cout << "initiliaze Level 1 " << std::endl;
      for(int i=0 ; i< a.size() ; i++ ) {
        //	std::cout << "i="<< i<< std::endl;
        a[i]=initFunctor(i);
      }
    }

    static inline void fillNoPad(ARRAY & a, typename ARRAY::RealType value){
      //      INFOS("LEVEL="<<1<<" a.size()="<<a.size());
      for(int i=0 ; i< a.size() ; i++ ) {
        a[i]=value;
      }
    }







  };


  template <class SCALAR_TYPE, int LEVEL, int PACK_SIZE=1, int PACK_LEVEL=1>
  class Array : public BaseArray< Array<SCALAR_TYPE,LEVEL,PACK_SIZE,PACK_LEVEL> >{

  public:
    // The raw aligned allocator used by Array never constructs nor destroys
    // elements: RealType must therefore be trivially copyable (memcpy/memset
    // based assignment and bulk copies are used throughout).
    static_assert(std::is_trivially_copyable<SCALAR_TYPE>::value,
                  "Legolas::Array<RealType> requires a trivially copyable RealType");

    typedef SCALAR_TYPE RealType;
    typedef ArrayShape<PACK_SIZE,PACK_LEVEL,LEVEL> Shape;
    typedef const ArrayShape<PACK_SIZE,PACK_LEVEL,LEVEL> & ConstShapeRef;
    static const int level=LEVEL;
    static const int packLevel=PACK_LEVEL;
    static const int packSize=PACK_SIZE;
    //    static const size_t stride_=Shape::stride_;
    typedef Array<SCALAR_TYPE,LEVEL-1,PACK_SIZE,PACK_LEVEL> Element;

    typedef Array StoreType;
    typedef ArrayRecursiveTraits<Array,LEVEL> ART;
    typedef typename ART::GetElement GetElement;
    typedef typename ART::ConstGetElement ConstGetElement;

    //    typedef typename ART::ConstGetElement ConstGetElement;

    Shape shape_;
    bool owner_;
    typedef Legolas::allocator<RealType> Allocator;
    RealType * dataPtr_;


    Array( void ):shape_(),
      owner_(false),
      dataPtr_(0)
    {
    }

    // Ownership transfer: the moved-from array must not keep owning the buffer,
    // otherwise both destructors would free the same allocation (double free).
    Array( Array && other ) noexcept:
      shape_(other.shape_),
      owner_(other.owner_),
      dataPtr_(other.dataPtr_)
    {
      other.owner_=false;
      other.dataPtr_=0;
    }


    static inline void dataInitialize(const Shape & s, RealType * dataPtr, Array & a){
      a.shape_=s;
      a.dataPtr_=dataPtr;
    }


    Array(const Shape & s):shape_(s),owner_(true),dataPtr_(0){
      //      data_.resize(shape_.dataSize_);
      //      dataPtr_=&data_[0];
      Allocator allocator;
      dataPtr_=allocator.allocate(shape_.dataSize_);
#ifdef ZEROINIT
      this->fill(RealType(0.0));
#endif


    }

    ~Array( void ){

      if (owner_){
        //	dataPtr_=0;
        //	if (dataPtr_!=0){
        Allocator allocator;
        allocator.deallocate(dataPtr_,shape_.dataSize_);
        dataPtr_=0;
        //	}
      }
    }



    Array(const Shape & shape, RealType * dataPtr_):shape_(shape),
      owner_(false),
      dataPtr_(dataPtr_){
    }

    //LP: Require delegating Ctor....
    template <typename... Args>
    Array(const Args&... rest):Array(Shape(rest...)){}

    //    template <typename... Args>
    //    Array(const Args... rest):shape_(Shape(rest...)),
    //			      owner_(true),
    //			      dataPtr_(0)
    //    {

    //      Allocator allocator;
    //      dataPtr_=allocator.allocate(shape_.dataSize_);
    //      data_.resize(shape_.dataSize_);
    //      dataPtr_=&data_[0];
    //    }


    RealType * realDataPtr( void ){ return dataPtr_;}
    const RealType * realDataPtr( void ) const { return dataPtr_;}


    int size( void ) const { return shape_.size_ ; }
    bool owner( void ) const { return owner_ ; }


    // It is a shallow copy (view)
    Array(Array & other):shape_(other.shape_),
      owner_(false),
      dataPtr_(other.dataPtr_){
      //      std::cout << "ici" << std::endl;
    }


    // It is a shallow copy (view)
    Array(const Array & other):shape_(other.shape_),
      owner_(false),
      dataPtr_(other.dataPtr_){
    }


    void reshape( void ){
      //      std::cout << "ici" << std::endl;
    }

    template <typename... Args>
    void reshape(const Args... rest){
      //      std::cout << "ici" << std::endl;
      Shape s(rest...);
      //      std::cout << "s.size()=" << s.size_ << std::endl;
      if (dataPtr_==0){
        shape_=s;
        owner_=true;
        Allocator allocator;
        dataPtr_=allocator.allocate(shape_.dataSize_);
#ifdef ZEROINIT
        this->fill(RealType(0.0));
#endif
      }
      else{
        std::cerr << "Reshape is not allowed" << std::endl;
        std::cerr << "dataPtr_="<<dataPtr_ << std::endl;
        throw std::runtime_error("Reshape is not allowed");
      }
    }

    typedef Legolas::NativeSimd<double,PACK_SIZE> PackedDoubleType;
    typedef Legolas::NativeSimd<SCALAR_TYPE,PACK_SIZE> PackedRealType;

    typedef Legolas::Array<PackedRealType,LEVEL,1,1> PackedArrayView;
    typedef Legolas::Array<PackedRealType,1> FlatPackedArrayView;

    const PackedArrayView getPackedView( void ) const {
      ArrayShape<1,1,LEVEL> packedShape=this->shape_.getPackedShape();
      PackedRealType * vectorPtr=reinterpret_cast<PackedRealType*>(this->dataPtr_);
      return PackedArrayView(packedShape,vectorPtr);
    }

    const Array<PackedRealType,1> getFlatPackedView( void ) const {
      typename Array<PackedRealType,1>::Shape shape(this->shape_.dataSize_/PACK_SIZE);
      return Array<PackedRealType,1>(shape,reinterpret_cast<PackedRealType*>(this->dataPtr_));
    }

    PackedArrayView getPackedView( void ) {
      ArrayShape<1,1,LEVEL> packedShape=this->shape_.getPackedShape();
      PackedRealType * vectorPtr=reinterpret_cast<PackedRealType*>(this->dataPtr_);
      return PackedArrayView(packedShape,vectorPtr);
    }

    Array<PackedRealType,1> getFlatPackedView( void ) {
      typename Array<PackedRealType,1>::Shape shape(this->shape_.dataSize_/PACK_SIZE);
      return Array<PackedRealType,1>(shape,reinterpret_cast<PackedRealType*>(this->dataPtr_));
    }

    typedef int ArrayView;

    inline size_t flatSize( void ) const { return this->shape_.dataSize_;}

    struct SetValue{
      RealType value_;

      SetValue(const RealType & value):value_(value){}

      RealType operator()(int i) const {
        return value_;
      }
    };


    void allFill(RealType value){
      if (this->flatSize() < Legolas::StaticScheduler::parallel_threshold_from_env()){
        std::fill(this->dataPtr_, this->dataPtr_ + this->flatSize(), value);
        return;
      }

      const size_t chunkSize=4*1000;
      const size_t nChunks=this->flatSize()/chunkSize;

      const int parallelChunks=std::max((int(nChunks)-1),0);

      Legolas::parallel_for(0,parallelChunks,[=](size_t i){
        const size_t begin=i*chunkSize;
        std::fill(this->dataPtr_ + begin, this->dataPtr_ + begin + chunkSize, value);
      });

      const size_t begin=parallelChunks*chunkSize;
      size_t lastChunkSize=this->flatSize()-begin;
      std::fill(this->dataPtr_ + begin, this->dataPtr_ + begin + lastChunkSize, value);
    }


    void fill(RealType value){
      this->allFill(value);
    }



    void fillNoPad(RealType value){
      ART::fillNoPad(*this,value);
    }


    struct CopyElement{
      template <class LEFT, class RIGHT>
      void operator()(int i, LEFT left, const RIGHT & right) const {
        left[i]=right[i];
      }
    };


    struct AddElement{
      template <class LEFT, class RIGHT>
      void operator()(int i, LEFT left, const RIGHT & right) const {
        left[i]+=right[i];
      }
    };

    struct MinusElement{
      template <class LEFT, class RIGHT>
      void operator()(int i, LEFT left, const RIGHT & right) const {
        left[i]-=right[i];
      }
    };


    Array & operator= (const Array & right){
      if (this->dataPtr_!=right.dataPtr_){
        assert(this->shape_==right.shape_);
        if (this->flatSize() < Legolas::StaticScheduler::parallel_threshold_from_env()){
          std::memcpy(this->dataPtr_, right.dataPtr_, this->flatSize() * sizeof(RealType));
          return *this;
        }

        const size_t chunkSize=4000;
        const size_t nChunks=this->flatSize()/chunkSize;
        const int parallelChunks=std::max((int(nChunks)-1),0);

        Legolas::parallel_for(0,parallelChunks,[=](size_t i){
          const size_t begin=i*chunkSize;
          std::memcpy(this->dataPtr_ + begin, right.dataPtr_ + begin, chunkSize * sizeof(RealType));
        });

        const size_t begin=parallelChunks*chunkSize;
        size_t lastChunkSize=this->flatSize()-begin;
        std::memcpy(this->dataPtr_ + begin, right.dataPtr_ + begin, lastChunkSize * sizeof(RealType));
      }
      return *this;
    }


    const Shape & shape( void ) const { return shape_;}


    template <class DERIVED>
    Array & operator= (const BaseArray<DERIVED> & right){
      assert(this->shape()==right.getArrayRef().shape());
      Legolas::flat_for_each(CopyElement(),(*this),right.getArrayRef());
      return *this;
    }

    template <class DERIVED>
    Array & operator= (const ParallelArray<DERIVED> & right){
      assert(this->shape()==right.getArray().shape());

      if (this->flatSize() < Legolas::StaticScheduler::parallel_threshold_from_env()){
        std::memcpy(this->dataPtr_, right.getArray().dataPtr_, this->flatSize() * sizeof(RealType));
        return *this;
      }

      const size_t chunkSize=4*1000;
      const size_t nChunks=this->flatSize()/chunkSize;

      const int parallelChunks=std::max((int(nChunks)-1),0);

      Legolas::parallel_for(0,parallelChunks,[=](size_t i){
        const size_t begin=i*chunkSize;
        std::memcpy(this->dataPtr_ + begin, right.getArray().dataPtr_ + begin, chunkSize * sizeof(RealType));
      });

      const size_t begin=parallelChunks*chunkSize;
      size_t lastChunkSize=this->flatSize()-begin;
      std::memcpy(this->dataPtr_ + begin, right.getArray().dataPtr_ + begin, lastChunkSize * sizeof(RealType));

      return *this;
    }

    template <class DERIVED>
    Array & operator+= (const BaseArray<DERIVED> & right){
      assert(this->shape()==right.getArrayRef().shape());
      Legolas::flat_for_each(AddElement(),(*this),right.getArrayRef());
      return *this;
    }

    template <class DERIVED>
    Array & operator+= (const ParallelArray<DERIVED> & right){
      assert(this->shape()==right.getArray().shape());
      Legolas::parallel_flat_for_each(AddElement(),(*this),right.getArray());
      return *this;
    }

    template <class DERIVED>
    Array & operator-= (const BaseArray<DERIVED> & right){
      assert(this->shape()==right.getArrayRef().shape());
      Legolas::flat_for_each(MinusElement(),(*this),right.getArrayRef());
      return *this;
    }

    template <class DERIVED>
    Array & operator-= (const ParallelArray<DERIVED> & right){
      assert(this->shape()==right.getArray().shape());
      Legolas::parallel_flat_for_each(MinusElement(),(*this),right.getArray());
      return *this;
    }


    //LP:FIXME who owns the data when Array b(a.copy()) is called..
    //Should not be a move Ctor involved ? It looks that the RVO is OK but I am not sure that it is guaranteed...
    Array copy( void ) const{
      Array result(this->shape_);
      result=(*this);
      return result;
    }

    // Array && copy( void ) const{
    //   INFOS("LA");
    //   Array result(this->shape_);
    //   INFOS("LA");
    //   result=(*this);
    //   INFOS("ICI");
    //   return std::move(result);
    // }


    template <typename... Args>
    void initialize(const Args... rest){
      ART::initialize(*this,rest...);
    }





    LEGOLAS_ALWAYS_INLINE GetElement operator[](int i){
#ifndef NDEBUG
      if ( (i<0) || (i>=this->size()) ) {
        INFOS("i="<<i<<"this->size()="<<this->size());
        assert( (i>0) && (i<this->size() ));
      }
#endif
      return ART::getElement(*this,i);
    }
    LEGOLAS_ALWAYS_INLINE ConstGetElement operator[](int i) const {
#ifndef NDEBUG
      if ( (i<0) || (i>=this->size()) ) {
        INFOS("i="<<i<<"this->size()="<<this->size());
        assert( (i>0) && (i<this->size() ));
      }
#endif
      //      assert(i>=0);
      //      assert(i<this->size());
      return ART::getElement(*this,i);
    }


    void display(std::ostream& os) const{
      std::string prefix;
      ART::display(*this,os,prefix);
    }

    void display(std::ostream& os, const std::string & prefix) const{
      ART::display(*this,os,prefix);
    }

    void copy(const Array & other){
      if (shape_!=other.shape_){
        this->reshape(other.shape_);
      }
      (*this)=other;
    }

    void save(std::ostream & os){
      this->shape_.save(os);
      os << std::endl;
      os << this->owner_;
      const size_t ss=this->shape_.dataSize_*sizeof(RealType);
      //      INFOS("ss="<<ss);
      os.write(reinterpret_cast<char*>(dataPtr_),ss);


      os << std::endl;
    }

    void load(std::istream & is){
      Shape shape;
      shape.load(is);
      this->reshape(shape);
      Legolas::dump(this->shape_);
      is >> this->owner_;
      //      INFOS("owner_="<<owner_);
      const size_t ss=this->shape_.dataSize_*sizeof(RealType);
      //      INFOS("ss="<<ss);
      is.read(reinterpret_cast<char*>(this->dataPtr_),ss);

    }


  };//End of Class Array.hxx

  inline void dump(const float & value){ std::cout << value <<" ";}

  template <class SCALAR_TYPE, int LEVEL, int PACK_SIZE, int PACK_LEVEL>
  inline void dump(const Legolas::Array<SCALAR_TYPE,LEVEL,PACK_SIZE,PACK_LEVEL> & array){
    std::cout <<"array.size()="<<array.size()<<std::endl;
    for (int i=0 ; i<array.size() ; i++){
      Legolas::dump(array[i]); std::cout << std::endl;
    }
  }


  template <class SCALAR_TYPE, int LEVEL, int PACK_SIZE, int PACK_LEVEL>
  std::ostream& operator<< ( std::ostream& os, const Legolas::Array<SCALAR_TYPE,LEVEL,PACK_SIZE,PACK_LEVEL> & a )
  {
    a.display(os);
    return os;
  }

  inline float exp(const float & value){ return std::exp(value); }

  template <class SCALAR_TYPE, int PACK_SIZE>
  inline auto  exp(const Legolas::NativeSimd<SCALAR_TYPE,PACK_SIZE> & pvalue ){
    Legolas::NativeSimd<SCALAR_TYPE,PACK_SIZE> res;
    for (size_t i = 0; i < PACK_SIZE; ++i) res[i] = std::exp(pvalue[i]);
    return res;
  }
  
  



}
