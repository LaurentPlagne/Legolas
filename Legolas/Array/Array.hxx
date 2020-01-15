#pragma once

#include <vector>
#include <iostream>
#include <Eigen/Core>
#include <stdexcept>
#include "Legolas/Allocator.hxx"
#include "Legolas/Array/ArrayShape.hxx"
#include "stringConvert.hxx"
#include "Legolas/Array/BaseArray.hxx"
#include "Legolas/Array/ForEach.hxx"
#include "Legolas/Array/Expression.hxx"
#include "Legolas/Array/Reductions.hxx"
#include "Legolas/StaticArray/StaticArray.hxx"

//#define ZEROINIT

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
    //    std::vector< RealType,Eigen::aligned_allocator<RealType> > data_;
    //std::vector< RealType > data_;
    typedef Legolas::allocator<RealType> Allocator;

    //typedef tbb::cache_aligned_allocator<RealType> Allocator;
    //typedef Eigen::aligned_allocator<RealType>  Allocator;
    //    typedef std::vector<RealType,tbb::cache_aligned_allocator<RealType> >Data;
    //    Data data_;
    RealType * dataPtr_;


    Array( void ):shape_(),
      owner_(false),
      dataPtr_(0)
    {
    }

    Array( Array && ) = default;


    static inline void dataInitialize(const Shape & s, RealType * dataPtr, Array & a){
      a.shape_=s;
      a.dataPtr_=dataPtr;
    }


    Array(const Shape & s):shape_(s),owner_(true),dataPtr_(0){
      //      data_.resize(shape_.dataSize_);
      //      dataPtr_=&data_[0];
      Allocator allocator;
      //      Eigen::aligned_allocator<RealType> allocator;
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
        //	Eigen::aligned_allocator<RealType> allocator;
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


    typedef Eigen::Array<double,PACK_SIZE,1> PackedDoubleType;
    typedef Eigen::Array<SCALAR_TYPE,PACK_SIZE,1> PackedRealType;


    typedef Legolas::Array<PackedRealType,LEVEL,1,1> PackedArrayView;
    typedef Legolas::Array<PackedRealType,1> FlatPackedArrayView;

    const PackedArrayView getPackedView( void ) const {
      // INFOS("ICI");
      // INFOS("this->shape_.size_="<<this->shape_.size_);
      ArrayShape<1,1,LEVEL> packedShape=this->shape_.getPackedShape();
      //      INFOS("ICI");
      PackedRealType * vectorPtr=reinterpret_cast<PackedRealType*>(this->dataPtr_);
      //      INFOS("ICI");
      return PackedArrayView(packedShape,vectorPtr);
    }

    const Array<PackedRealType,1> getFlatPackedView( void ) const {
      //LP:Rq a.shape_.dataSize_/PACK_SIZE is equal to npack that could be stored in the Shape
      typename Array<PackedRealType,1>::Shape shape(this->shape_.dataSize_/PACK_SIZE);
      return Array<PackedRealType,1>(shape,reinterpret_cast<PackedRealType*>(this->dataPtr_));
    }


    PackedArrayView getPackedView( void ) {
      ArrayShape<1,1,LEVEL> packedShape=this->shape_.getPackedShape();
      PackedRealType * vectorPtr=reinterpret_cast<PackedRealType*>(this->dataPtr_);
      return PackedArrayView(packedShape,vectorPtr);
    }

    Array<PackedRealType,1> getFlatPackedView( void ) {
      //LP:Rq a.shape_.dataSize_/PACK_SIZE is equal to npack that could be stored in the Shape
      typename Array<PackedRealType,1>::Shape shape(this->shape_.dataSize_/PACK_SIZE);
      return Array<PackedRealType,1>(shape,reinterpret_cast<PackedRealType*>(this->dataPtr_));
    }

    // typedef EigenInterface<SCALAR_TYPE,PACK_SIZE> EI;
    // typedef typename EI::EigenArray EigenArray;
    // typedef typename EI::EigenArrayView EigenView;
    // typedef typename EI::ConstEigenArrayView ConstEigenView;



    //typedef Eigen::Map<EigenArray,Eigen::Aligned> EigenView;
    //typedef Eigen::Map<const EigenArray,Eigen::Aligned> ConstEigenView;

    typedef Eigen::Array<RealType,Eigen::Dynamic,1> EigenArray;
    typedef Eigen::Map<EigenArray,Eigen::Aligned32> EigenView;
    typedef Eigen::Map<const EigenArray,Eigen::Aligned32> ConstEigenView;
    //    typedef Eigen::Map<EigenArray,Eigen::Aligned> EigenView;
    //    typedef Eigen::Map<const EigenArray,Eigen::Aligned> ConstEigenView;
    //typedef Eigen::Map<EigenArray> EigenView;
    //    typedef Eigen::Map<const EigenArray> ConstEigenView;


    typedef int ArrayView;

    inline size_t flatSize( void ) const { return this->shape_.dataSize_;}

    const EigenView getEigenView( void ) const {
      return EigenView(this->dataPtr_,this->flatSize());
    }

    inline EigenView getEigenView( void ) {
      return EigenView(this->dataPtr_,this->flatSize());
    }

    inline ConstEigenView getEigenView(size_t begin, size_t chunkSize) const {
      //      ConstEigenView toto(this->dataPtr_+begin,chunkSize);
      //      INFOS("ICI");
      return ConstEigenView(this->dataPtr_+begin,chunkSize);
    }

    inline EigenView getEigenView(size_t begin, size_t chunkSize) {
      //      EigenView toto(this->dataPtr_+begin,chunkSize);
      //      INFOS("LA");
      return EigenView(this->dataPtr_+begin,chunkSize);
    }



    struct SetValue{
      RealType value_;

      SetValue(const RealType & value):value_(value){}

      RealType operator()(int i) const {
        //	LEFT v(value_);
        return value_;
      }
    };


    void allFill(RealType value){
      const size_t chunkSize=4*1000;
      const size_t nChunks=this->flatSize()/chunkSize;

      const int parallelChunks=std::max((int(nChunks)-1),0);

      my_tbb::parallel_for(0,parallelChunks,[=](size_t i){
        const size_t begin=i*chunkSize;
        this->getEigenView(begin,chunkSize).fill(value);
      });

      const size_t begin=parallelChunks*chunkSize;
      size_t lastChunkSize=this->flatSize()-begin;
      this->getEigenView(begin,lastChunkSize).fill(value);


      //      this->getEigenView().fill(value);
    }


    void fill(RealType value){
      this->allFill(value);
      //      this->getEigenView().fill(value);
    }



    void fillNoPad(RealType value){
      //this->getEigenView().fill(value);
      ART::fillNoPad(*this,value);

      //      ART::initialize(*this,SetValue(value));



      //            Legolas::for_each(CopyElement(),(*this).getFlatPackedView());
      //      this->getEigenView().fill(value);
      //      Array<PackedRealType,1> flat=this->getFlatPackedView();
      //      PackedRealType pvalue(value);
      //      for (int i=0 ; i<flat.size() ; i++) flat[i]=pvalue;


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
        //	INFOS("left[i]="<<left[i]<<" right[i]="<<right[i]);
        left[i]-=right[i];
        //	INFOS("left[i]="<<left[i]<<" right[i]="<<right[i]);
      }
    };


    Array & operator= (const Array & right){
      if (this->dataPtr_!=right.dataPtr_){
        assert(this->shape_==right.shape_);
        const size_t chunkSize=4000;
        const size_t nChunks=this->flatSize()/chunkSize;
        const int parallelChunks=std::max((int(nChunks)-1),0);

        //	size_t begin=0;
        my_tbb::parallel_for(0,parallelChunks,[=](size_t i){
          const size_t begin=i*chunkSize;
          //	    void * dest=static_cast<void *>(this->realDataPtr()+begin);
          //	    const void * source=static_cast<const void *>(right.realDataPtr()+begin);
          //	    A_memcpy(dest,source,chunkSize*sizeof(RealType));
          this->getEigenView(begin,chunkSize)=right.getEigenView(begin,chunkSize);
        });

        const size_t begin=parallelChunks*chunkSize;
        size_t lastChunkSize=this->flatSize()-begin;

        //	INFOS("begin="<<begin);
        //	INFOS("lastChunkSize="<<lastChunkSize);
        //	auto titi=this->getEigenView(begin,lastChunkSize);
        //	INFOS("LA");
        //	auto tata=right.getEigenView(begin,chunkSize);

        this->getEigenView(begin,lastChunkSize)=right.getEigenView(begin,lastChunkSize);


        //	this->getEigenView()=right.getEigenView();
      }


      //	Legolas::for_each(CopyElement(),(*this).getFlatPackedView(),right.getFlatPackedView());
      //LP: OK with C++14
      //Legolas::for_each([=](int i, auto l, auto r){l[i]=r[i];},(*this).getFlatPackedView(),right.getFlatPackedView());

      //      }
      return *this;
    }


    const Shape & shape( void ) const { return shape_;}


    template <class DERIVED>
    Array & operator= (const BaseArray<DERIVED> & right){
      assert(this->shape()==right.getArrayRef().shape());
      //this->getEigenView()=right.getEigenView();
      //      INFOS("ICI");
      // auto r=right.getArrayRef();
      // const size_t s=r.size();
      //      INFOS("s="<<s);

      this->getEigenView()=right.getArrayRef().getEigenView();



      //      for (size_t i=0 ; i<s ; i++){
      //      	(*this)[i]=r[i];
      //      }

      //      Legolas::flat_for_each(CopyElement(),(*this),right.getArrayRef());
      // #ifdef CXX14
      //       Legolas::flat_for_each([](int i, auto l, auto r){l[i]+=r[i];},(*this),right.getArrayRef());
      // #else
      return *this;
    }

    template <class DERIVED>
    Array & operator= (const ParallelArray<DERIVED> & right){
      assert(this->shape()==right.getArray().shape());
      //      INFOS("ICI LA");

      //      this->getEigenView(0,this->flatSize())=right.getArrayRef().getEigenView(0,this->flatSize());

      const size_t chunkSize=4*1000;
      const size_t nChunks=this->flatSize()/chunkSize;
      //      INFOS("nChunks="<<nChunks);
      //      INFOS("this->flatSize()-chunkSize*nChunks="<<this->flatSize()-chunkSize*nChunks);

      //      size_t begin=0;

      const int parallelChunks=std::max((int(nChunks)-1),0);

      my_tbb::parallel_for(0,parallelChunks,[=](size_t i){
        const size_t begin=i*chunkSize;
        this->getEigenView(begin,chunkSize)=right.getArrayRef().getEigenView(begin,chunkSize);
      });

      // for (size_t i=0 ; i+1<nChunks ; i++){
      // 	size_t begin=i*chunkSize;
      // 	//	INFOS("begin="<<begin);
      // 	this->getEigenView(begin,chunkSize)=right.getArrayRef().getEigenView(begin,chunkSize);
      // 	//      	begin+=chunkSize;
      // }



      //      size_t begin=std::max((int(nChunks)-1),0)*chunkSize;
      const size_t begin=parallelChunks*chunkSize;
      //      INFOS("begin="<<begin);
      size_t lastChunkSize=this->flatSize()-begin;
      //      INFOS("lastChunkSize="<<lastChunkSize);

      this->getEigenView(begin,lastChunkSize)=right.getArrayRef().getEigenView(begin,lastChunkSize);


      //      Legolas::parallel_flat_for_each(CopyElement(),(*this),right.getArray());
      //      auto l=this->getFlatPackedView();
      //      auto r=right.getArray().getFlatPackedView();

      //      Legolas::parallel_flat_for_each(CopyElement(),(*this),right.getArray());
      //      right.getArray().getEigenView();
      //      this->getEigenView();
      //      this->getEigenView()=right.getArray().getEigenView();
      //      auto l=this->getEigenView();
      //      auto r=right.getArray().getEigenView();



      //      l=r;

      //      tbb::parallel_for(0,this.eigenView().size(),[=](size_t i){algo(i,rest...);});

      //      Legolas::parallel_flat_for_each(CopyElement(),(*this).getEigenView(),right.getArray().getEigenView());
      return *this;
    }

    template <class DERIVED>
    Array & operator+= (const BaseArray<DERIVED> & right){
      assert(this->shape()==right.getArrayRef().shape());
      this->getEigenView()+=right.getArrayRef().getEigenView();
      //      Legolas::flat_for_each(AddElement(),(*this),right.getArrayRef());
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
      //      INFOS("****************** IXI *********************");
      assert(this->shape()==right.getArrayRef().shape());
      this->getEigenView()-=right.getArrayRef().getEigenView();
      //      Legolas::flat_for_each(MinusElement(),(*this),right.getArrayRef());
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





    GetElement operator[](int i){
#ifndef NDEBUG
      if ( (i<0) || (i>=this->size()) ) {
        INFOS("i="<<i<<"this->size()="<<this->size());
        assert( (i>0) && (i<this->size() ));
      }
#endif
      return ART::getElement(*this,i);
    }
    ConstGetElement operator[](int i) const {
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
  inline auto  exp(const Eigen::Array<SCALAR_TYPE,PACK_SIZE,1> & pvalue ){ return Eigen::exp(pvalue); }
  
  



}
