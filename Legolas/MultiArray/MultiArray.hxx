#pragma once

#include "Legolas/Allocator.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Vector/VirtualVector.hxx"
#include "Legolas/MultiArray/MultiArrayShape.hxx"

//#define ZEROINIT

namespace Legolas{

  template <class MULTI_ARRAY, int LEVEL>
  struct MultiArrayRecursiveTraits{

    typedef std::size_t SizeType;

    typedef typename MULTI_ARRAY::SubMultiArray Element;
    typedef typename MULTI_ARRAY::SubMultiArray & GetElement;
    typedef const typename MULTI_ARRAY::SubMultiArray & ConstGetElement;

    //    static inline appendElement(MULTI_ARRAY & ma, ConstGetElement element){
    //      ma.data_.push_back(element);
    //    }

    static inline GetElement getElement(MULTI_ARRAY & ma, SizeType i){
      assert(i>=0) ;
      assert(i<ma.data().size());
      return ma.data()[i];}
    static inline ConstGetElement getElement(const MULTI_ARRAY & ma, SizeType i){
      assert(i>=0) ;
      assert(i<ma.data().size());
      return ma.data()[i];
    }

    static inline       Element & getVirtualElement(      MULTI_ARRAY & vec, SizeType i){ return vec[i];}
    static inline const Element & getVirtualElement(const MULTI_ARRAY & vec, SizeType i){ return vec[i];}

  };

  template <class MULTI_ARRAY>
  struct MultiArrayRecursiveTraits<MULTI_ARRAY,1>{

    typedef std::size_t SizeType;

    //    typedef typename MULTI_ARRAY::Array  Element;
    typedef typename MULTI_ARRAY::Array Element;
    typedef typename MULTI_ARRAY::Array & GetElement;
    typedef const typename MULTI_ARRAY::Array & ConstGetElement;

    static inline const MULTI_ARRAY & getVirtualElement(const MULTI_ARRAY & vec, SizeType i){ INFOS("ERROR") ; return vec;}
    static inline       MULTI_ARRAY & getVirtualElement(MULTI_ARRAY & vec, SizeType i){ INFOS("ERROR") ; return vec;}


    //    static inline GetElement getElement(MULTI_ARRAY & ma, SizeType i){ return *(ma.data()[i]);}
    //    static inline ConstGetElement getElement(const MULTI_ARRAY & ma, SizeType i){ return *(ma.data()[i]);}
  };

  template <class ARRAY, int LEVEL>
  class MultiArray: public BaseArray< MultiArray<ARRAY,LEVEL> >, public VirtualVector {
  public:

    typedef std::size_t SizeType;

    static const int level=LEVEL+ARRAY::level;
    //static const int level=LEVEL;
    static const int packSize=ARRAY::packSize;
    typedef typename ARRAY::RealType RealType;
    typedef Legolas::allocator<RealType> Allocator;
    //typedef tbb::cache_aligned_allocator<RealType> Allocator;
    //    typedef Eigen::aligned_allocator<RealType>  Allocator;
    //fake types for Array Expression compatibility
    //    static const int packLevel=-1;
    static const int packLevel=ARRAY::packLevel;
    //    typedef SizeType Shape;
    //    typedef SizeType ConstShapeRef;
    typedef MultiArray & PackedArrayView;
    typedef typename ARRAY::FlatPackedArrayView FlatPackedArrayView;
    static const int PACK_SIZE=ARRAY::packSize;

    typedef Eigen::Array<RealType,Eigen::Dynamic,1> EigenArray;
    //    typedef Eigen::Map<EigenArray,Eigen::Aligned> EigenView;
    //    typedef Eigen::Map<const EigenArray,Eigen::Aligned> ConstEigenView;
    typedef Eigen::Map<EigenArray,Eigen::Aligned32> EigenView;
    typedef Eigen::Map<const EigenArray,Eigen::Aligned32> ConstEigenView;

    //end fake types

    typedef typename ARRAY::PackedRealType PackedRealType;
    typedef typename ARRAY::PackedDoubleType PackedDoubleType;


    typedef const MultiArray & StoreType;
    typedef ARRAY Array;
    typedef MultiArray<Array,LEVEL-1> SubMultiArray;
    typedef MultiArrayRecursiveTraits<MultiArray,LEVEL> MART;

    typedef typename MART::Element Element;
    typedef typename MART::GetElement GetElement;
    typedef typename MART::ConstGetElement ConstGetElement;


    typedef MultiArrayShape<typename ARRAY::Shape,LEVEL> Shape;


    bool owner_;
    Shape shape_;
    RealType * realDataPtr_;
    std::vector<Element> data_;

    const Shape & shape( void ) const { return shape_ ; }


    MultiArray( void ):owner_(false),
		       shape_(),
		       realDataPtr_(0),
		       data_(){
    }


    ~MultiArray( void ){
      if (owner_){
	this->realDataDeAllocate(shape_.flatSize());
      }
    }


    void realDataDeAllocate(SizeType flatSize ){
      if (realDataPtr_){
	Allocator allocator;
	allocator.deallocate(realDataPtr_,flatSize);
      }
    }

    RealType * realDataAllocate(SizeType flatSize){
      Allocator allocator;

      RealType * result=allocator.allocate(flatSize);

	//      return

      //LP:fixME


      // for (SizeType i=0 ; i<flatSize ; i++){
      // 	result[i]=0.0;
      // }

      return result;


    }

    RealType * realDataPtr( void ) const { return realDataPtr_;}

    static inline void dataInitialize(const Shape & shape, RealType * realDataPtr, MultiArray & ma){
      ma.shape_=shape;
      ma.data_.resize(ma.shape_.size());
      ma.realDataPtr_=realDataPtr;
      for (SizeType i=0 ; i<ma.data_.size() ; i++){
	Element::dataInitialize(ma.shape_[i],realDataPtr,ma.data_[i]);
	realDataPtr+=ma.shape_[i].flatSize();
      }
#ifdef ZEROINIT
      ma.fill(0.0);
#endif
    };




    MultiArray(const Shape & shape):owner_(true),
				    shape_(),
				    realDataPtr_(this->realDataAllocate(shape.flatSize())),
				    data_()
    {
      //      INFOS("shape.size()="<<shape.size());
      dataInitialize(shape,realDataPtr_,*this);
    }

    MultiArray & operator= (const MultiArray & other){
      this->copy(other);
      return *this;
    }


    MultiArray(const MultiArray & other):owner_(true),
					 shape_(other.shape()),
					 realDataPtr_(realDataAllocate(shape_.flatSize())),
					 data_()
    {
      //      INFOS("ici");
      //      throw std::runtime_error("ici");
      dataInitialize(shape_,realDataPtr_,*this);
      //      this->copy(other);
      (*this)=other;

    }




    void reshape(const Shape & shape){
      if (shape!=shape_){
	if (!realDataPtr_) owner_=true;
	if (owner_){
	  this->realDataDeAllocate(shape_.flatSize());
	  realDataPtr_=this->realDataAllocate(shape.flatSize());
	  dataInitialize(shape,realDataPtr_,*this);
	  //	  this->dataInitialize(shape,realDataPtr_);
	}
	else{
	  //	  INFOS("shape="<<shape_);
	  //	  INFOS("source.shape_="<<shape);
	  INFOS("No reshape is allowed");
	  throw std::runtime_error("No reshape is allowed");
	}
      }
    }


    GetElement operator[]( SizeType i) {
#ifndef NDEBUG
      if ( (i<0) || (i>=this->size()) ) {
	INFOS("i="<<i<<"this->size()="<<this->size());
	assert( (i>0) && (i<this->size() ));
      }
#endif
      return data_[i] ;
    }
    ConstGetElement operator[]( SizeType i) const {
#ifndef NDEBUG
      if ( (i<0) || (i>=this->size()) ) {
	INFOS("i="<<i<<"this->size()="<<this->size());
	assert( (i>0) && (i<this->size() ));
      }
#endif
      return data_[i] ;
    }

    SizeType size( void ) const { return shape_.size() ;}

    // template <typename... Args>
    // MultiArray(SizeType first, const Args... rest):data_(first)
    // {
    //   for (SizeType i=0 ; i< this->size() ; i++) data_[i].reshape(rest...);
    // }

    // MultiArray( void ):dataPtr_(0){
    // }

    // MultiArray(const MultiArrayShape & shape):shape_(shape),dataPtr_(0),data_(){

    // }

    // MultiArray(const MultiArray & other){
    //   this->copy(other);
    // }

    // void reshape(const MultiArrayShape & mas){
    //   mas_=mas;
    //   data_.resize(mas.size());
    //   for (int i=0 ; i< mas.size() ; i++){
    // 	data_[i].reshape(mas[i]);
    //   }
    // }


    // void resize(SizeType size){
    //   data_.resize(size);
    // }

    // template <typename... Args>
    // void reshape(SizeType first, const Args... rest){
    //   data_.resize(first);
    //   for (SizeType i=0 ; i< this->size() ; i++){
    // 	data_[i].reshape(rest...);
    //   }
    // }

    void copy(const MultiArray & other){
      //      INFOS("Copy ICI LEVEL="<<LEVEL<<" level="<<level);
      if (data_.size()!=other.size()){
	INFOS("********* ACTUAL ALLOC **************");
	data_.resize(other.size());
      }
      this->getArrayView()=other.getArrayView();

      //      this->getEigenView()=other.getEigenView();
      //      for (SizeType i=0 ; i< other.size() ; i++){
      //	(*this)[i].copy(other[i]);
      //      }
    }

    void fill(RealType value){
      this->getArrayView().fill(value);
      //      this->getEigenView().fill(value);
      //      for (SizeType i=0 ; i<this->size() ; i++){
      //      	(*this)[i].fill(value);
      //      }
    }

    void fillNoPad(RealType value){
      for (SizeType i=0 ; i<this->size() ; i++){
	(*this)[i].fillNoPad(value);
      }
    }

    void allFill(RealType value){
      this->getArrayView().fill(value);
      //      this->getEigenView().fill(value);
      //      for (SizeType i=0 ; i<this->size() ; i++){
      //      	(*this)[i].fill(value);
      //      }
    }


    const EigenView getEigenView( void ) const {
      //LP:Rq a.shape_.dataSize_/PACK_SIZE is equal to npack that could be stored in the Shape
      //      typedef Eigen::Array<RealType,Eigen::Dynamic,1> VE;
      //      typedef Eigen::Map<VE,Eigen::Aligned> MVE;
      //      MVE mve(this->realDataPtr_,this->flatSize());
      //      typename EigenView::Shape shape(this->shape_.flatSize()/PACK_SIZE);
      //      return EigenView(shape,reinterpret_cast<PackedRealType*>(this->realDataPtr_));
      return EigenView(this->realDataPtr_,this->flatSize());
    }

    EigenView getEigenView( void ) {
      //LP:Rq a.shape_.dataSize_/PACK_SIZE is equal to npack that could be stored in the Shape
      //      typename EigenView::Shape shape(this->shape_.flatSize()/PACK_SIZE);
      //      return EigenView(shape,reinterpret_cast<PackedRealType*>(this->realDataPtr_));
      return EigenView(this->realDataPtr_,this->flatSize());
    }

    inline ConstEigenView getEigenView(size_t begin, size_t chunkSize) const {
      //      ConstEigenView toto(this->dataPtr_+begin,chunkSize);
      //      INFOS("ICI");
      return ConstEigenView(this->realDataPtr_+begin,chunkSize);
    }

    inline EigenView getEigenView(size_t begin, size_t chunkSize) {
      //      EigenView toto(this->dataPtr_+begin,chunkSize);
      //      INFOS("LA");
      return EigenView(this->realDataPtr_+begin,chunkSize);
    }



    typedef Legolas::Array<RealType,1,packSize,1> ArrayView;

    const ArrayView getArrayView( void ) const {
      typename ArrayView::Shape shape(this->flatSize());
      return ArrayView(shape,this->realDataPtr_);
    }

    ArrayView getArrayView( void ){
      typename ArrayView::Shape shape(this->flatSize());
      return ArrayView(shape,this->realDataPtr_);
    }



    SizeType flatSize( void ) const { return shape_.flatSize();}

    template <class DERIVED>
    MultiArray & operator= (const BaseArray<DERIVED> & right){
      // //LP:fixme assert same shape !
      // INFOS("ICI");
      // this->getArrayView();
      // INFOS("LA");
      // right.getArrayRef().getArrayView();
      // INFOS("HERE");
      // this->getArrayView()=right.getArrayRef().getArrayView();
      // //      this->getEigenView()=right.getArrayRef().getEigenView();

      // const DERIVED & r=right.getArrayRef();
      // for (SizeType i=0 ; i<this->size() ; i++){
      // 	(*this)[i]=r[i];
      // }
      //      for (int i=0 ; i<this.size() ; i++){

      (*this).getEigenView()=right.getArrayRef().getEigenView();


      return *this;
    }

#ifdef MULTI_ARRAY_ON_EXPRESSION
    template <class DERIVED>
    MultiArray & operator= (const ParallelArray<DERIVED> & right){
      INFOS("LA");
      this->getArrayView()=right.getArrayRef().getArrayView() || par;
      //LP:fixme assert same shape !
      //      this->getFlatPackedView()=right.getArrayRef().getFlatPackeView() || par;
      //      this->getEigenView()=right.getArrayRef().getEigenView() || par;
      //      Legolas::parallel_flat_for_each(typename Array::CopyElement(),(*this).getEigenView(),right.getArray().getEigenView());
      return *this;
    }


    template <class DERIVED>
    MultiArray & operator+= (const BaseArray<DERIVED> & right){
      this->getEigenView()+=right.getArrayRef().getEigenView();
      //LP:fixme assert same shape !
      //      const DERIVED & r=right.getArrayRef();
      //      for (SizeType i=0 ; i<this->size() ; i++){
      //	(*this)[i]+=r[i];
      //      }
      return *this;
    }

    template <class DERIVED>
    MultiArray & operator-= (const BaseArray<DERIVED> & right){
      //LP:fixme assert same shape !
      this->getEigenView()-=right.getArrayRef().getEigenView();
      //      INFOS("Legolas::squaredNorm(r)="<<Legolas::squaredNorm(right));
      //      const DERIVED & r=right.getArrayRef();

      //      INFOS("Legolas::squaredNorm(r)="<<Legolas::squaredNorm(r));
      //      for (SizeType i=0 ; i<this->size() ; i++){
      //	(*this)[i]-=r[i];
      //      }
      return *this;
    }



    template <class DERIVED>
    MultiArray & operator+= (const ParallelArray<DERIVED> & right){
      //LP:fixme assert same shape !
      this->getEigenView()+=right.getArrayRef().getEigenView() || par;
      return *this;
    }

    template <class DERIVED>
    MultiArray & operator-= (const ParallelArray<DERIVED> & right){
      //LP:fixme assert same shape !
      this->getEigenView()-=right.getArrayRef().getEigenView() || par;
      return *this;
    }

#endif




    // void push_back(ConstGetElement element){
    //   std::cout << "avant this->size()=" << this->size() << std::endl ;

    //   this->data_.push_back();
    //   std::cout << "ici" << (*this) << std::endl;

    //   std::cout << "apres this->size()=" << this->size() << std::endl ;
    //   std::cout << "apres this->size()=" << this->size() << std::endl ;


    //   this->data_[this->size()-1].copy(element);

    //   std::cout << "la" << (*this) << std::endl;

    //   std::cout << "ensuite this->size()=" << this->size() << std::endl ;
    // }




    // void copyIn( void ) const {
    //   MultiArray result;
    //   result.data_.resize(this->size());
    //   for (SizeType i=0 ; i< this->size() ; i++){
    // 	result[i]=this->data_[i].copy();
    //   }

    //   std::cout << "copy res" << result ;

    //   return result;
    // }


    // stream operator
    // expression template +=Expr
    // fill or = operator
    // norm2 and dot


    void display(std::ostream& os, const std::string & prefix) const{
      //      os << " -- realDataPtr_=";
      //      os << vec.realDataPtr() << std::endl;
      os << std::endl;
      //      INFOS("vec.size()="<<vec.size());

      const MultiArray & vec=*this;;

      for (SizeType i=0 ; i < vec.size() ; i++){
	std::string indexString(prefix);
	indexString+="[\t";
	indexString+=stringConvert(i);
	indexString+="\t]";
	vec[i].display(os,indexString);
      }

      return ;
    }

    void display(std::ostream& os) const{
      std::string prefix;
      this->display(os,prefix);
    }

    //VirtualVector API

    //    int size( void ) const { return int(this->size()) ; }
          VirtualVector & getElement( SizeType i )       { return MART::getVirtualElement(*this,i);}
    const VirtualVector & getElement( SizeType i ) const { return MART::getVirtualElement(*this,i);}

    VirtualVector * clone( void ) const{
      return new MultiArray(*this) ;
    }

    VirtualVector * newVirtualVector( const VirtualVectorShape &  shape) const{
      INFOS("not Yet implemented");
      throw std::runtime_error("Not yet implemented");
      return 0;
    }

    virtual bool sameShape(const VirtualVector & source) const {
      //FIXME
      //      INFOS("not Yet implemented");
      //      throw std::runtime_error("Not yet implemented");
      return true;
    }

    void copy(const VirtualVector & source){
      return this->copy(static_cast<const MultiArray &>(source));
    }

    void scale(double a){
      //      if (a!=0.0){
	this->getEigenView()*=a;
	//      }
	//      else{
	//	INFOS("ICI");
	//	this->getEigenView().setZero();
	//      }
      //      this->getEigenView()=a*this->getEigenView();
      //      if (std::abs(a-1.0)>1.e-8) (*this)=(*this)*a;
    }


    inline double relativeDiffAndCopy(const VirtualVector & source){
      double sumDiff=0.0;
      double sumRight=0.0;
      MultiArray & left=static_cast<MultiArray &>(*this);
      const MultiArray & right=static_cast<const MultiArray &>(source);

      for (size_t i=0 ; i<left.size() ; i++){

        //left[i]-=right[i];
        //sumDiff+=Legolas::squaredNorm(left[i]);
        //sumDiff+=Legolas::squaredNorm(left[i]-right[i]);
	//sumRight+=Legolas::squaredNorm(right[i]);
        sumDiff+=Legolas::squaredNormAssumeZeroPadding(left[i]-right[i]);
	sumRight+=Legolas::squaredNormAssumeZeroPadding(right[i]);

	left[i]=right[i];

	//	sum+=relativeDiffAndCopy(left[i],right[i]);
      }

      return sumDiff/sumRight;

    }

    inline void plusAssign(double factor, const VirtualVector & source){
      MultiArray & left=static_cast<MultiArray &>(*this);
      const MultiArray & right=static_cast<const MultiArray &>(source);
      left.getEigenView()+=factor*right.getEigenView();
    }

    inline void scaleAndPlusAssign(double scaleFactor, double factor, const VirtualVector & source){
      MultiArray & left=static_cast<MultiArray &>(*this);
      const MultiArray & right=static_cast<const MultiArray &>(source);
      //      INFOS("FIXME§!!!!!!!!!!!!!!!!!!!!!!!");
      left.getEigenView()=scaleFactor*left.getEigenView()+factor*right.getEigenView();
    }

    inline double dot(const VirtualVector & r) const {
      const MultiArray & right=static_cast<const MultiArray &>(r);
      const MultiArray & left=static_cast<const MultiArray &>(*this);
      return Legolas::dot(left,right);
    }

    void display( void ) const{
      std::string prefix;
      this->display(std::cout,prefix);
    }

    //Fin Virtual Vector API

    void swap(MultiArray & other){
      RealType * temp=this->realDataPtr_;
      this->realDataPtr_=other.realDataPtr_;
      other.realDataPtr_=temp;
      this->data_.swap(other.data_);
    }


    void save(std::ostream & os){
      os << owner_ << std::endl;
      this->shape_.save(os);
      const SizeType ss=this->shape_.flatSize()*sizeof(RealType);
      //      INFOS("ss="<<ss);
      os.write(reinterpret_cast<char*>(realDataPtr_),ss);
      //      char * toto=reinterpret_cast<char*>(realDataPtr_);
      //      for (SizeType i=0 ; i<std::min(SizeType(20),ss) ; i++){
      //       	INFOS("toto["<<i<<"]="<<toto[i]);
      //      }
      //      os << std::endl;
    }

    void load(std::istream & is){
      is >> this->owner_;
      //      INFOS("owner_="<<owner_);
      assert(owner_==true);
      Shape shape;
      shape.load(is);
      this->reshape(shape);
      const SizeType ss=this->shape_.flatSize()*sizeof(RealType);
      //      INFOS("ss="<<ss);
      is.get();//for the std::endl a la fin de la shape
      is.read(reinterpret_cast<char*>(this->realDataPtr_),ss);
      //      char * toto=reinterpret_cast<char*>(realDataPtr_);
      //      for (SizeType i=0 ; i<std::min(SizeType(20),ss) ; i++){
      //	INFOS("toto["<<i<<"]="<<toto[i]);
      //      }
    }

    //    typedef typename ARRAY::PackedRealType PackedRealType;
    //    typedef typename ARRAY::FlatPackedArrayView FlatPackedArrayView;
    
    const FlatPackedArrayView getFlatPackedView( void ) const {
      assert(shape_.flatSize()%PACK_SIZE==0);
      
      //LP:Rq a.shape_.dataSize_/PACK_SIZE is equal to npack that could be stored in the Shape
      typename  FlatPackedArrayView::Shape shape(this->shape_.flatSize()/PACK_SIZE);
      return FlatPackedArrayView(shape,reinterpret_cast<PackedRealType*>(this->realDataPtr_));
    }



  };

  template <class ARRAY, int LEVEL>
  void swap(Legolas::MultiArray<ARRAY,LEVEL> & a,Legolas::MultiArray<ARRAY,LEVEL> & b){
    a.swap(b);
  }

template <class ARRAY, int LEVEL>
inline double dot(const Legolas::MultiArray<ARRAY,LEVEL> & right, const Legolas::MultiArray<ARRAY,LEVEL> & left){
  double sum=0.0;
  for (size_t i=0 ; i<right.size() ; i++){
    sum+=Legolas::dot(right[i],left[i]);
  }
  return sum;
}



  // template <class DERIVED>
  // inline typename DERIVED::RealType squaredNorm(const BaseArray<DERIVED> & ba){
  //   //    std::cout <<" ICIIIIIIIIIIIIIIIIIIII" << std::endl;
  //   typename DERIVED::RealType accumulator(0.0);
  //   auto a=ba.getArrayRef();
  //   if (DERIVED::level!=DERIVED::packLevel){
  //     //      std::cout << "DERIVED::level="<<DERIVED::level<<" "<<DERIVED::packLevel<<std::endl;
  //     for (int i=0 ; i<a.size(); i++){
  // 	accumulator+=squaredNorm(a[i]);
  // 	//accumulator+=a[i]*a[i];
  //     }
  //   }


  template <class ARRAY, int LEVEL>
  std::ostream& operator<< ( std::ostream& os, const Legolas::MultiArray<ARRAY,LEVEL> & a )
  {
    a.display(os);
    return os;
  }

//   template <class ARRAY, int LEVEL>
// inline double dotAssumeZeroPadding(const MultiArray<ARRAY,LEVEL> & left, const MultiArray<ARRAY,LEVEL> & right){
//   double sum=0.0;
//   for (int i=0 ; i<left.size() ; i++){
//     sum+=dotAssumeZeroPadding(left[i],right[i]);
//   }
//   return sum;
// }


  // template <class ARRAY, int LEVEL>
  // double squaredNormZeroPad(const Legolas::MultiArray<ARRAY,LEVEL> & a){
  //   double sum=0.0;
  //   for (size_t i=0 ; i<a.size() ; i++){
  //     sum+=Legolas::squaredNormZeroPad(a[i]);
  //   }
  //   return sum;
  // }

  // template <class DERIVED>
  // double squaredNorm(const BaseArray<DERIVED> & ba){
  //   double result=0.0;
  //   auto a=ba.getArrayRef();
  //   for (SizeType i=0 ; i<a.size() ; i++){
  //     result+=squaredNorm(a[i]);
  //   }
  // }

}
