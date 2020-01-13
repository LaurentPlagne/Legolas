#ifndef __LEGOLAS_RECTANGULARMULTIVECTOR_HXX__
#define __LEGOLAS_RECTANGULARMULTIVECTOR_HXX__

#include "UTILITES.hxx"
#include "Legolas/Allocator.hxx"
#include "Legolas/EigenInterface.hxx"
#include "Legolas/Vector/TinyEigenInterface.hxx"
#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/Common/BasicTypes.hxx"
#include "Legolas/Vector/Assign/AssignMode.hxx"
#include "Legolas/Vector/Assign/Assign.hxx"
#include "Legolas/Vector/Expression/MatrixVectorExpression.hxx"
#include "Legolas/Vector/Expression/VectorExpression.hxx"
#include "Legolas/Vector/Expression/VectorOperations.hxx"
#include "Legolas/Vector/Shape/ShapeTraits.hxx"
#include "Legolas/Vector/Backtrace.hxx"
#include "stringConvert.hxx"
#include "X86Timer.hxx"
#include "RecursiveTraits.hxx"
#include "Legolas/Vector/ForAll.hxx"
#include "Legolas/Vector/VirtualVector.hxx"
#include "my_tbb_parallel_for.hxx"


namespace Legolas{

  //   class VirtualVector{};

  //   class BlockVector : public VirtualVector {
  //   public:
  //     //    virtual int getSize( void ) const  = 0 ;
  //     virtual int size( void ) const  = 0 ;
  //     virtual const VirtualVector & getElement( int i ) const = 0 ;
  //     virtual VirtualVector & getElement( int i ) = 0 ;

  //     virtual BlockVector * clone( void ) = 0 ;
  //   };

  /**************************************************************************************************/

#define POWER_OF_TWO 12


  //  template <class SCALAR_TYPE>
  inline void assignValueToScalar(const double & value, double & target){ target=value;}
  inline void assignValueToScalar(const double & value, float & target){ target=value; }
  inline void assignValueToScalar(const double & value, int & target){
    INFOS("ZARBI");
    target=value;
  }

  //  template <class SCALAR_TYPE>
  //  void convertToScalar(const float & value){return value ; }


  inline double dot(const double & left, const double & right){
    return left*right;
  }

  inline double dot(const float & left, const float & right){
    return left*right;
    //    return double(left)*double(right);
  }

  inline double dot(const int & left, const int & right){
    INFOS("ZZZZ");
    return left*right;
  }
  // __attribute__((no_sanitize_memory))
  // inline  std::ifstream myOpenfile(std::string dataFilename){
  //   return inputfile(dataFilename.c_str(),std::ios::in | std::ios::binary );
  // }

  //  __attribute__((no_sanitize_memory))
  inline void myReadFile(std::string dataFilename, float * buffer, uint ss){
    std::ifstream inputfile(dataFilename.c_str(),std::ios::in | std::ios::binary );
    inputfile.read(reinterpret_cast<char*>(buffer),ss);
    inputfile.close();
  }

  //  __attribute__((no_sanitize_memory))
  inline void myReadFile(std::string dataFilename, double * buffer, uint ss){
    std::ifstream inputfile(dataFilename.c_str(),std::ios::in | std::ios::binary );
    inputfile.read(reinterpret_cast<char*>(buffer),ss);
    inputfile.close();
  }





// #ifdef LEGOLAS_USE_EIGEN
//   const int packSize_=4;
//   typedef Eigen::Array<float, packSize_, 1> BlockArray;

//   template <>
//   inline BlockArray convertToScalar<BlockArray>(const double & value){return BlockArray::Constant(value) ; }


//   inline double dot(const BlockArray & right, const BlockArray & left){
//     return right.matrix().dot(left.matrix());
//   }

// #endif


  template <class DERIVED_LEFT, class DERIVED_RIGHT>
  inline double sequential_dot(size_t begin,
			       size_t length,
			       const BaseVector<DERIVED_LEFT> & left,
			       const BaseVector<DERIVED_RIGHT> & right){
    const DERIVED_LEFT  & l=left.getCVR();
    const DERIVED_RIGHT & r=right.getCVR();

#ifdef LEGOLAS_USE_EIGEN
    return l.eigenVectorView(begin,length).matrix().dot(r.eigenVectorView(begin,length).matrix());
#else
    double result=0.0;
    const size_t end=begin+length;
    for (size_t i=begin ; i<end ; i++){
      result+=Legolas::dot(l(i),r(i));
    }
    return result;
#endif
  }



  template <class EXPR_L, class EXPR_R>
  class DotFunctor {
  public:
    DotFunctor(const EXPR_L & l,
	       const EXPR_R & r):left_(l),right_(r),sum_(0){}

    inline void operator()( const my_tbb::blocked_range<size_t>& r ) {

      const size_t b=r.begin();
      const size_t e=r.end();

      const size_t length=(e-b)<<POWER_OF_TWO;
      const size_t firstIndex=b<<POWER_OF_TWO;

      // INFOS("b="<<b);
      // INFOS("e="<<e);
      // INFOS("firstIndex="<<firstIndex);
      // INFOS("length="<<length);

      //      sum_+=Legolas::sequential_dot(left_.eigenVectorView(firstIndex,length).matrix().dot(right_.eigenVectorView(firstIndex,length).matrix());
      sum_+=Legolas::sequential_dot(firstIndex,length,left_,right_);

      //      for( size_t i=b; i!=e; ++i ){
      //	sum_ += left_(i)*right_(i);
      //      }
    }

    DotFunctor(DotFunctor & x, my_tbb::split ) : left_(x.left_),
						 right_(x.right_),
						 sum_(0.0) {}

      void join( const DotFunctor & y ) {sum_+=y.sum_;}

  private:
    const EXPR_L & left_;
    const EXPR_R & right_;
  public:
    double sum_;
  };

  /*******************************************************************************************/


  template <class DERIVED_LEFT, class DERIVED_RIGHT>
  inline double dot(const BaseVector<DERIVED_LEFT> & left,
		    const BaseVector<DERIVED_RIGHT> & right){

    const DERIVED_LEFT  & l=left.getCVR();
    const DERIVED_RIGHT & r=right.getCVR();

    assertSameShapes(l.shape(),r.shape());

    const size_t nbBlocks=l.flatSize()>>POWER_OF_TWO; //division par 1024



    DotFunctor<DERIVED_LEFT,DERIVED_RIGHT> df(l,r);

    my_tbb::parallel_reduce(my_tbb::blocked_range<size_t>(0,nbBlocks), df );

    double result=df.sum_;

    const size_t smallBlockBegin=nbBlocks<<POWER_OF_TWO;

    if (smallBlockBegin<l.flatSize()){
      const size_t smallBlockLength=l.flatSize()-smallBlockBegin;
      //      result+=l.eigenVectorView(smallBlockBegin,smallBlockLength).matrix().dot(r.eigenVectorView(smallBlockBegin,smallBlockLength).matrix());
      result+=Legolas::sequential_dot(smallBlockBegin,smallBlockLength,l,r);
    }

    return result;

  }



  // class VirtualVector {
  // public:
  //   //    virtual int getSize( void ) const  = 0 ;
  //   virtual int size( void ) const  = 0 ;
  //   virtual const VirtualVector & getElement( int i ) const = 0 ;
  //   virtual VirtualVector & getElement( int i ) = 0 ;
  //   virtual VirtualVector * clone( void ) const = 0 ;
  //   virtual VirtualVector * newVirtualVector( const VirtualVectorShape & shape) const = 0 ;
  //   virtual ~VirtualVector( void ){};

  //   virtual bool sameShape(const VirtualVector & source ) const = 0;
  //   virtual void copy(const VirtualVector & source) = 0 ;
  //   virtual void scale( double a ) =0 ;
  //   virtual double relativeDiffAndCopy( const VirtualVector & source ) = 0;
  //   virtual void plusAssign(double factor, const VirtualVector & source) = 0;
  //   virtual double dot(const VirtualVector & source) const = 0;
  //   virtual void scaleAndPlusAssign(double scaleFactor, double factor,const VirtualVector & source) = 0;

  //   virtual void display( void ) const =0;

  //   typedef VirtualVector * VirtualVectorPtr;
  //   static inline VirtualVector & getClone(VirtualVectorPtr & vecPtr,const VirtualVector & source){
  //     if (!vecPtr){
  // 	vecPtr=source.clone();
  //     }
  //     else{
  // 	if (!vecPtr->sameShape(source)){
  // 	  delete vecPtr;
  // 	  vecPtr=source.clone();
  // 	}
  // 	else{
  // 	  vecPtr->copy(source);
  // 	}
  //     }
  //     return (*vecPtr);
  //   }


  // };

  //  inline double dot(const VirtualVector & l,const VirtualVector & r){ return l.dot(r);}


  /** \brief Classe principale d'implémentation des vecteurs Legolas++

      Cette classe correspond à la classe principale d'implémentation des vecteurs Legolas++. Les autres classes du
      répertoire Legolas/Vector implémentent des sous-parties de cette classe.

      \ingroup VectorGroup
  */
  template <class SCALAR_TYPE, int LEVEL>
  class RectangularMultiVector : public BaseVector< RectangularMultiVector<SCALAR_TYPE,LEVEL> >, public VirtualVector{
  public:

    typedef std::size_t SizeType;

    typedef RecursiveTraits<RectangularMultiVector,
			    RectangularMultiVector<SCALAR_TYPE,LEVEL-1>,
			    SCALAR_TYPE,LEVEL>    RCT;
    typedef typename RCT::Element               Element;
    typedef SCALAR_TYPE                           RealType;

    typedef Element &                           GetElement;
    typedef const Element &                     ConstGetElement;

    typedef typename ShapeTraits<LEVEL>::Type   Shape;

    typedef const RectangularMultiVector &      StoreType;

    typedef  std::vector<RealType,Legolas::allocator<RealType> >  ScalarDataVector;




#ifdef LEGOLAS_USE_EIGEN

    typedef typename EigenInterface<RealType>::EigenVectorView EigenVectorView;

    inline EigenVectorView eigenVectorView( void ){ return EigenVectorView(realDataPtr_,flatSize_) ; }
    inline const EigenVectorView eigenVectorView( void ) const { return EigenVectorView(realDataPtr_,flatSize_) ; }

    inline EigenVectorView eigenVectorView(size_t begin, size_t length){ return EigenVectorView(realDataPtr_+begin,length) ; }
    inline const EigenVectorView eigenVectorView(size_t begin, size_t length) const { return EigenVectorView(realDataPtr_+begin,length) ; }

#endif


    //typedef Legolas::SequentialForAll ForAllElements;
    typedef ParallelBlockedForAll ForAllElements;

    RectangularMultiVector( void ):realDataPtr_(0),owner_(false),dataPtr_(0),shape_(),flatSize_(0){}

    RectangularMultiVector(std::string s):realDataPtr_(0),
					  owner_(true),
					  dataPtr_(0),
					  shape_(),
					  flatSize_(0)
    {
      Shape shape(s);
      this->realDataAllocate(shape);
      this->realDataInitialize();
      this->initialize(shape,realDataPtr_,flatSize_);
    }

    RectangularMultiVector(const Shape & shape):realDataPtr_(0),
						owner_(true),
						dataPtr_(0),
						shape_(),
						flatSize_(0)
    {
      this->realDataAllocate(shape);
      //      INFOS("flatSize="<<flatSize_);
      this->realDataInitialize();
      //      INFOS("flatSize="<<flatSize_);
      this->initialize(shape,realDataPtr_,flatSize_);
      //      INFOS("flatSize="<<flatSize_);
    }


    RectangularMultiVector(const Shape & shape,RealType * realDataPtr):realDataPtr_(realDataPtr),//rehape
								       owner_(false),
								       dataPtr_(0),
								       shape_(),
								       flatSize_(0)
    {
      shape_=shape;
      flatSize_=shape.flatSize();
      //      flatSize_=Legolas::getFlatSize(shape);
      //      this->realDataAllocate(shape);
      //      this->realDataInitialize();
      this->initialize(shape,realDataPtr_,flatSize_);
    }


    void realDataAllocate(const Shape & shape){
      shape_=shape;
      //      flatSize_=Legolas::getFlatSize(shape);
      flatSize_=shape.flatSize();
      //      INFOS("flatSize="<<Legolas::getFlatSize(shape));
      //      INFOS("flatSize="<<flatSize_);
#ifdef CACHE_ALLOCATOR
      scalarDataVectorPtr_=new ScalarDataVector(flatSize_);
      realDataPtr_=&(*scalarDataVectorPtr_)[0];
#else
      realDataPtr_=new RealType[flatSize_];
#endif
      //      realDataPtr_=new RealType[flatSize_];
      //      INFOS("ICI");
    }



    void realDataInitialize( void ){
      //      INFOS("ici");
      this->nullify();
      //      (*this)=0.0;
      //      Assign<EqualAssign,RealType>::apply(flatSize_,RealValueAdapter(RealType(0.0)),realDataPtr_);
    }

    RectangularMultiVector(const RectangularMultiVector & source):realDataPtr_(0),
								  owner_(true),
								  dataPtr_(0),
								  shape_(),
								  flatSize_(0)
    {
      //      INFOS("ICI");
      if (source.realDataPtr_){

	this->realDataAllocate(source.shape_);
	//	INFOS("ICI");
	this->initialize(source.shape_,realDataPtr_,flatSize_);
	ForAllElements::apply(BinaryRangeFunctorFactory<EqualAssign>::apply(*this,source),0,flatSize_);
	//	this->eigenVectorView()=source.eigenVectorView();
	//	Assign<EqualAssign,RealType>::apply(flatSize_,source.realDataPtr_,realDataPtr_);
	//	BL::copy(flatSize_,source.realDataPtr_,realDataPtr_);
      }
    }

 //    template <class DERIVED>
//     static inline Shape buildShapeFromBaseVector(const BaseVector<DERIVED> & right) {
//       const DERIVED & r=right.getCVR();
//       return RCT::buildShapeFromBaseVector(r);
//     }

    //Constructor from a Vector Expression
    template <class DERIVED>
    RectangularMultiVector(const BaseVector<DERIVED> & right):realDataPtr_(0),
							      owner_(true),
							      dataPtr_(0),
							      shape_(),
							      flatSize_(0)
    {
      const DERIVED & r=right.getCVR();
      //      this->realDataAllocate(RectangularMultiVector::buildShapeFromBaseVector(right));
      this->realDataAllocate(RCT::buildShapeFromBaseVector(r));
      this->initialize(shape_,realDataPtr_,flatSize_);
      (*this)=r;
    }

    ~RectangularMultiVector( void ){
      if (dataPtr_){
	delete [] dataPtr_;
	dataPtr_=0;
      }

      if (owner_){
	if (realDataPtr_){
#ifdef CACHE_ALLOCATOR
	  realDataPtr_=0;
	  delete  scalarDataVectorPtr_;
#else
	  delete [] realDataPtr_;
#endif
	  //	  delete [] realDataPtr_;
	}
      }
    }


    void initialize(const Shape & shape, RealType * realDataPtr,size_t flatSize){
      RCT::initialize(shape,realDataPtr,flatSize,*this);
    }

    class CopyFunctor{
    private:
      const RealType * sPtr_;
      RealType * tPtr_;

    public:
      CopyFunctor(RealType * s,RealType * t):sPtr_(s),
					     tPtr_(t){}

      inline void operator() (size_t i) const { tPtr_[i]=sPtr_[i];}
    };


    //   if (source.realDataPtr_){
// 	if (shape_!=source.shape_){
// 	  this->reshape(source.shape_);
// 	}
// 	//VectorLoop::apply(flatSize_,CopyFunctor(source.realDataPtr_,realDataPtr_));
// 	Assign<EqualAssign,RealType>::apply(flatSize_,source.realDataPtr_,realDataPtr_);
// 	//	BL::copy(flatSize_,source.realDataPtr_,realDataPtr_);
//       }
//       return (*this);
//    }



    RectangularMultiVector & inject (const RectangularMultiVector & source){
      if (shape_==source.shape_){
	this->operator=(source);
      }
      else{
	SizeType thisSize=std::min(this->size(),source.size());
	for (SizeType i=0 ; i<thisSize ; i++){
	  Legolas::inject(source[i],(*this)[i]);
	}
      }
      return (*this);
    }


    void reshape(const Shape & shape){
      if (shape_!=shape){
	if (!realDataPtr_) owner_=true;
	if (owner_){	//	owner_=true;
	  if (realDataPtr_){
	    //	    delete[] realDataPtr_;
#ifdef CACHE_ALLOCATOR
	    delete scalarDataVectorPtr_;
	    realDataPtr_=0;
#else
	    delete[] realDataPtr_;
	    realDataPtr_=0;
#endif

	  }
	  if (dataPtr_){
	    delete[] dataPtr_;
	    dataPtr_=0;
	  }
	  this->realDataAllocate(shape);
	  this->realDataInitialize();
	  this->initialize(shape,realDataPtr_,flatSize_);
	}
	else{	//	owner_=false;

	  INFOS("shape="<<shape_);
	  INFOS("source.shape_="<<shape);
	  INFOS("No reshape is allowed");
	  throw std::runtime_error("No reshape is allowed");
	}
      }
    }

    void resize(const SizeType size){
      RCT::resize(size,*this);
    }


    SizeType size( void ) const {return RCT::size(shape_) ; }

    bool isAllocated( void ) const { return true ;}

    inline void assertIndexInBounds(const SizeType & i) const{
#if DK_HAS_DEBUG != 0
      if ( (i>=this->size()) || (i<0) ){
	INFOS("INDEX i="<<i<<" is out of bounds and should be in [0, "<<this->size()<<"[");
	INFOS("this->shape()="<<this->shape());
	BACKTRACE
	throw std::runtime_error("Index out of bound");
      }
#endif
    }

    Element & operator[](SizeType i){
      assertIndexInBounds(i);
      return RCT::getElement((*this),i);
    }
    const Element & operator[](SizeType i)const{
      assertIndexInBounds(i);
      return RCT::getElement((*this),i);
    }

    inline const RealType & operator()(size_t i) const { return this->realDataPtr()[i];}
    inline RealType & operator()(size_t i) { return this->realDataPtr()[i];}


    /* ______________________ Common vectorial methods ___________________________  */

    // class RealValueAdapter{
    // private:
    //   RealType value_;
    // public:
    //   RealValueAdapter(const RealType & value):value_(value){}
    //   inline RealType operator[](size_t i)const{return value_;}
    // };


    // class AssignValueFunctor{
    // private:
    //   RealType alpha_;
    //   RealType * tPtr_;

    // public:
    //   AssignValueFunctor(const RealType & alpha,RealType * t):alpha_(alpha),
    // 						      tPtr_(t){}

    //   inline void operator() (size_t i) const { tPtr_[i]=alpha_;}
    // };

 //    class AssignZeroFunctor{
//     private:
//       RealType alpha_;
//       RealType * tPtr_;

//     public:
//       AssignZeroFunctor(const RealType & alpha,RealType * t):alpha_(alpha),
// 							      tPtr_(t){}

//       inline void operator() (SizeType i) const { tPtr_[i]=alpha_;}
//     };



    inline void sequential_nullify(size_t begin, size_t length) {
      std::memset(realDataPtr_+begin,0,length*sizeof(RealType));
    }

    inline void nullify( void ){
      ForAllElements::apply(SetZeroFunctor< RectangularMultiVector<RealType,LEVEL> >(*this),0,flatSize_);
    }

    template <class A_SCALAR_TYPE>
    RealType convertToScalar(A_SCALAR_TYPE value){
      RealType v;
      Legolas::assignValueToScalar(value,v);
      return v;
    }


    template <class A_SCALAR_TYPE>
    RectangularMultiVector & setScalar(A_SCALAR_TYPE value){
#define NULLIFY
#ifdef NULLIFY
      if (std::abs(value)>=1.e-12){
	//	const ScalarAdapter<RealType> sa(convertToScalar(value));
	ForAllElements::apply(BinaryRangeFunctorFactory<EqualAssign>::apply(*this,ScalarAdapter<RealType>(convertToScalar(value))),0,flatSize_);
	//	const RealType v=convertToScalar<A_SCALAR_TYPE>(value);
	//	ForAllElements::apply(BinaryRangeFunctorFactory<EqualAssign>::apply(*this,source),0,flatSize_);
	//	const size_t thisSize=this->size();
	//	for (size_t i=0 ; i<thisSize ; i++){
	//	  (*this)[i]=v;
	//	}
      }
      else{
	this->nullify();
      }
#else
      ForAllElements::apply(BinaryRangeFunctorFactory<EqualAssign>::apply(*this,ScalarAdapter<RealType>(convertToScalar(value))),0,flatSize_);
#endif
      return *this;
    }

    RectangularMultiVector & operator = (float value){ return this->setScalar(value) ; }
    RectangularMultiVector & operator = (double value){ return this->setScalar(value) ; }

    RectangularMultiVector & operator *= (double value){
      ForAllElements::apply(BinaryRangeFunctorFactory<MultAssign>::apply(*this,ScalarAdapter<RealType>(convertToScalar(value))),0,flatSize_);
      return *this;
    }

    RectangularMultiVector & operator *= (float value){
      ForAllElements::apply(BinaryRangeFunctorFactory<MultAssign>::apply(*this,ScalarAdapter<RealType>(convertToScalar(value))),0,flatSize_);
      return *this;
    }



    RectangularMultiVector & operator = (const RectangularMultiVector & source){
      if (source.realDataPtr_){
	//	if (!realDataPtr_) this->reshape(source.shape_); //Reshape is only allowed for empty vector (from default Ctor)
	this->reshape(source.shape_);
	assertSameShapes(shape_,source.shape_);

	ForAllElements::apply(BinaryRangeFunctorFactory<EqualAssign>::apply(*this,source),0,flatSize_);
	//	this->eigenVectorView()=source.eigenVectorView();
	//Assign<EqualAssign,RealType>::apply(flatSize_,source.realDataPtr_,realDataPtr_);
      }
      else{
	if (realDataPtr_){
	  INFOS("This operation is not allowed !!");
	  throw std::runtime_error("This operation is not allowed");
	}
      }
      return (*this);
    }


    inline void sequential_copy(const RectangularMultiVector & source,size_t begin, size_t length){
      //      INFOS("sequential copy");
      void * to=static_cast<void *>(realDataPtr_+begin);
      const void * from=static_cast<const void *>(source.realDataPtr_+begin);
      size_t len=length*sizeof(RealType);
      memcpy(to,from,len);
    }






    //Expression Template
    template <class DERIVED>
    RectangularMultiVector & operator = (const BaseVector<DERIVED> & right){
      const DERIVED & r=right.getCVR();
      assertSameShapes(this->shape(),r.shape());
      //      eigenVectorView(r);
      //      this->eigenVectorView()=eigenVectorView(r);
      ForAllElements::apply(BinaryRangeFunctorFactory<EqualAssign>::apply(*this,r),0,flatSize_);
      //      this->eigenVectorView()=r.eigenVectorView();
      return *this;
    }

    //Expression Template
    template <class DERIVED>
    RectangularMultiVector & operator += (const BaseVector<DERIVED> & right){
      const DERIVED & r=right.getCVR();
      assertSameShapes(this->shape(),r.shape());
      //      eigenVectorView(r);
      //      this->eigenVectorView()+=eigenVectorView(r);
      ForAllElements::apply(BinaryRangeFunctorFactory<PlusAssign>::apply(*this,r),0,flatSize_);
      //this->eigenVectorView()+=r.eigenVectorView();
      return *this;
    }

    //Expression Template
    template <class DERIVED>
    RectangularMultiVector & operator -= (const BaseVector<DERIVED> & right){
      const DERIVED & r=right.getCVR();
      assertSameShapes(this->shape(),r.shape());
      ForAllElements::apply(BinaryRangeFunctorFactory<MinusAssign>::apply(*this,r),0,flatSize_);
      //      this->eigenVectorView()-=eigenVectorView(r);
      //      this->eigenVectorView()-=r.eigenVectorView();
      return *this;
    }



    //Matrix Operations...


    template<class LEFT, class OPERATOR, class RIGHT>
    void operator = (const MatrixVectorExpression<LEFT,OPERATOR,RIGHT> & expression){
      //      assertSameShapes(this->shape(),expression.shape());
      typedef typename OPERATOR::template Engine< EqualAssign > Operator;
      Operator::apply(expression.matrixOperand(),expression.vectorOperand(),(*this));
    }


    template<class LEFT, class OPERATOR, class RIGHT>
    void operator += (const MatrixVectorExpression<LEFT,OPERATOR,RIGHT> & expression){
      //      assertSameShapes(this->shape(),expression.shape());
      typedef typename OPERATOR:: template Engine<PlusAssign> Operator;
      Operator::apply(expression.matrixOperand(),expression.vectorOperand(),(*this));
    }

    template<class LEFT, class OPERATOR, class RIGHT>
    void operator -= (const MatrixVectorExpression<LEFT,OPERATOR,RIGHT> & expression){
      //      assertSameShapes(this->shape(),expression.shape());
      typedef typename OPERATOR::template Engine< MinusAssign > Operator;
      Operator::apply(expression.matrixOperand(),expression.vectorOperand(),(*this));
    }


    void operator = (const AnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().matrixVectorProduct(expression.vectorOperand(),(*this),0);
    }

    void operator += (const AnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().matrixVectorProduct(expression.vectorOperand(),(*this),1);
    }

    void operator -= (const AnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().matrixVectorProduct(expression.vectorOperand(),(*this),-1);
    }


    void operator = (const AnyMatrixVectorInverseExpression & expression){
      expression.matrixOperand().matrixVectorInverse(expression.vectorOperand(),(*this));
    }

    void operator = (const TransposeAnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().transposeMatrixVectorProduct(expression.vectorOperand(),(*this),0);
    }

    void operator += (const TransposeAnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().transposeMatrixVectorProduct(expression.vectorOperand(),(*this),1);
    }

    void operator -= (const TransposeAnyMatrixVectorMultExpression & expression){
      expression.matrixOperand().transposeMatrixVectorProduct(expression.vectorOperand(),(*this),-1);
    }

//     void operator = (const TransposeAnyMatrixVectorInverseExpression & expression){
//       expression.matrixOperand().transposeMatrixVectorInverse(expression.vectorOperand(),(*this));
//    }


    /* ______________________ Common vectorial methods ___________________________  */


    typedef RealType * RealPtr;
    typedef Element  * ElementPtr;

    //    const RealType & getRef( void ) const { return *realDataPtr_ ;}
    const RealPtr & getRef( void ) const { return realDataPtr_ ;}


    inline RealPtr       & realDataPtr( void )       {return realDataPtr_;}
    inline const RealPtr & realDataPtr( void ) const {return realDataPtr_;}
    bool             & owner( void )       { return owner_ ;}
    const       bool & owner( void ) const { return owner_ ;}
    ElementPtr       & dataPtr( void )       {return dataPtr_;}
    const ElementPtr & dataPtr( void ) const {return dataPtr_;}

    Shape       & shape( void )       {return shape_;}
    Shape shape( void ) const {return shape_;}

    size_t        & flatSize( void )       {return flatSize_;}
    const size_t  & flatSize( void ) const {return flatSize_;}

    double sizeInMB( void ) const { return this->flatSize()*sizeof(RealType)/(1024.*1024);}

    void display(std::ostream& os) const {
      std::string prefix;
      RCT::display(*this,os,prefix);
    }

    void display( void ) const {
      this->display(std::cout);
    }

    void display(std::ostream& os, const std::string & prefix) const {
      RCT::display(*this,os,prefix);
    }

    void dump(std::ofstream& os, const std::string & prefix) const {
      RCT::dump(*this,os,prefix);
    }



    void dump(std::string filename) const {
      INFOS("filename="<<filename);
      const SizeType MAXDISP=1000000;
      if (this->flatSize()<MAXDISP){
	std::ofstream outfile(filename.c_str(),std::ios::out);
	RCT::dump(*this,outfile,std::string());
	//	outfile.close();
      }
      else{
	INFOS("this->flatSize()="<<this->flatSize()<<" > "<<MAXDISP<<" dump file is skipped");
      }
    }


    bool operator != (const RectangularMultiVector & r) const {
      return !((*this)==r);
    }


    bool operator == (const RectangularMultiVector & r) const {
      bool isEqual=( (owner_==r.owner_) &&
		     (shape_==r.shape_) &&
		     (flatSize_==r.flatSize_) );
      if (isEqual){
	for (size_t i=0 ; i< flatSize_ ; i++){
	  isEqual=(isEqual && (realDataPtr_[i]==r.realDataPtr_[i]));
	}
      }

      if (isEqual && dataPtr_){
	for (SizeType i=0 ; i< this->size() ; i++){
	  isEqual=(isEqual && (dataPtr_[i]==r.dataPtr_[i]));
	}
      }

      return isEqual;
    }

    inline int level( void ) const {return LEVEL;}
    //    static const SizeType level=LEVEL;


    typedef typename RCT::VirtualElement  VirtualElement;

    const VirtualElement & getElement( SizeType i ) const {
      //      INFOS("not yet implemented !!!");
      return RCT::getVirtualElement(*this,i);
      //      return static_cast<VirtualVector &>(this->operator[](i)) ;
    }

    VirtualElement & getElement( SizeType i ) {
      //      INFOS("not yet implemented !!!");
      return RCT::getVirtualElement(*this,i);
      //      return *this;
    }



    VirtualVector * clone( void ) const{
      return new  RectangularMultiVector(*this) ;
    }

    VirtualVector * newVirtualVector( const VirtualVectorShape &  shape) const{
      return new  RectangularMultiVector(static_cast<const Shape &>(shape)) ;
    }

    virtual bool sameShape(const VirtualVector & source) const {
      return this->sameShape(static_cast<const RectangularMultiVector &>(source));
    }

    bool sameShape(const RectangularMultiVector & source) const{
      return (this->shape()==source.shape());
    }

    void copy(const VirtualVector & source){
      return this->copy(static_cast<const RectangularMultiVector &>(source));
    }

    void copy(const RectangularMultiVector & source){
      (*this)=source;
    }

    void scale(double a){
      if (std::abs(a-1.0)>1.e-8) (*this)*=a;
    }

    void save(std::ostream & os) const {
      this->shape().save(os);
      SizeType ss=this->flatSize()*sizeof(RealType);
      os.write(reinterpret_cast<char*>(realDataPtr_),ss);
    }
    void load(std::istream & is){
      Shape shape;
      shape.load(is);
      this->reshape(shape);
      SizeType ss=this->flatSize()*sizeof(RealType);
      is.get();//for the std::endl a la fin de la shape
      is.read(reinterpret_cast<char*>(this->realDataPtr_),ss);
    }
    
    void save(const std::string filename) const {
      std::string shapeFilename(filename+"_Shape.dat");
      INFOS("Save Vector with Shape :"<<shape_);
      this->shape().dump(shapeFilename);

      std::string dataFilename(filename+"_Data.dat");
      std::ofstream outfile(dataFilename.c_str(),std::ios::out | std::ios::binary);
      SizeType ss=this->flatSize()*sizeof(RealType);
      outfile.write(reinterpret_cast<char*>(realDataPtr_),ss);
      outfile.close();
    }

    void load(const std::string filename){
      INFOS("Loading from file "<<filename);
      assert(owner_ || (realDataPtr_==0));
      std::string shapeFilename(filename+"_Shape.dat");
      Shape shape;
      //      INFOS("ICI");
      shape.read(shapeFilename);
      INFOS("Load Vector with Shape :"<<shape);
      this->reshape(shape);
      //      INFOS("ICI");
      std::string dataFilename(filename+"_Data.dat");
      //      std::ifstream inputfile(dataFilename.c_str(),std::ios::in | std::ios::binary );

      //      std::ifstream inputfile=myOpenFile(dataFilename);
      //      INFOS("ICI");
      SizeType ss=this->flatSize()*sizeof(RealType);
      //      INFOS("ICI");
      myReadFile(dataFilename,realDataPtr_,ss);
      //      INFOS("ICI");
      //      inputfile.read(reinterpret_cast<char*>(realDataPtr_),ss);
      //      inputfile.close();
    }




    class RelativeDiffAndCopyFunctor {
    public:
      RelativeDiffAndCopyFunctor(RealType * l,
				 const RealType * r):left_(l),right_(r),sum_(0),sumx_(0){}

      inline void operator()( const my_tbb::blocked_range<size_t>& r ) {
	for( size_t i=r.begin(); i!=r.end(); ++i ){
	  //	  const double xi=right_[i];
	  //	  const double diff=left_[i]-xi;

	  const RealType & xi=right_[i];
	  const RealType diff=left_[i]-xi;
	  //	  sum_ += diff*diff;
	  //	  sumx_+= xi*xi;

	  sum_ += Legolas::dot(diff,diff);
	  sumx_+= Legolas::dot(xi,xi);
	  left_[i]=right_[i];
	}
      }

      RelativeDiffAndCopyFunctor(RelativeDiffAndCopyFunctor & x, my_tbb::split ) : left_(x.left_),
										   right_(x.right_),
										   sum_(0.0) ,
										   sumx_(0.0)
      {}

      void join( const RelativeDiffAndCopyFunctor & y ) {
	sum_+=y.sum_;
	sumx_+=y.sumx_;
      }

    private:
      RealType * left_;
      const RealType * right_;
    public:
      double sum_;
      double sumx_;
    };


    inline double relativeDiffAndCopy(const VirtualVector & source){
      const RectangularMultiVector & right=static_cast<const RectangularMultiVector &>(source);

      assertSameShapes(this->shape(),right.shape());

      RelativeDiffAndCopyFunctor ff(this->realDataPtr(),right.realDataPtr());
      my_tbb::parallel_reduce(my_tbb::blocked_range<size_t>(0,this->flatSize()), ff);
      return sqrt(ff.sum_/ff.sumx_);

    }

    inline void plusAssign(double factor, const VirtualVector & source){
      RectangularMultiVector & left=static_cast<RectangularMultiVector &>(*this);
      const RectangularMultiVector & right=static_cast<const RectangularMultiVector &>(source);
      left+=factor*right;
    }

    inline void scaleAndPlusAssign(double scaleFactor, double factor, const VirtualVector & source){
      RectangularMultiVector & left=static_cast<RectangularMultiVector &>(*this);
      const RectangularMultiVector & right=static_cast<const RectangularMultiVector &>(source);
      //      INFOS("FIXME§!!!!!!!!!!!!!!!!!!!!!!!");
      left=scaleFactor*left+factor*right;
    }

    inline double dot(const VirtualVector & r) const {
      const RectangularMultiVector & right=static_cast<const RectangularMultiVector &>(r);
      const RectangularMultiVector & left=static_cast<const RectangularMultiVector &>(*this);
      return Legolas::dot(left,right);
    }


  private:

    RealType * realDataPtr_;
    ScalarDataVector * scalarDataVectorPtr_;
    bool owner_;
    Element * dataPtr_;
    Shape shape_;
    size_t flatSize_;
  };

  /**************************************************************************************************/

  template <class SCALAR_TYPE, int LEVEL>
  std::ostream& operator<< ( std::ostream& os, const RectangularMultiVector<SCALAR_TYPE,LEVEL> & v )
  {
    v.display(os);
    return os;
  }

  template <class EXPR>
  class SquareNorm2Functor {
  public:
    SquareNorm2Functor(const EXPR & expr):expr_(expr),sum_(0.0){}

    inline void operator()( const my_tbb::blocked_range<size_t>& r ) {
      const size_t b=r.begin();
      const size_t e=r.end();
      for( size_t i=b; i!=e; ++i ){
	const typename EXPR::RealType ex=expr_(i);
	sum_ += Legolas::dot(ex,ex);
      }
    }

    SquareNorm2Functor(SquareNorm2Functor & x, my_tbb::split ) : expr_(x.expr_),
								 sum_(0.0) {}

    void join( const SquareNorm2Functor & y ) {sum_+=y.sum_;}

  private:
    const EXPR & expr_;
  public:
    double sum_;
  };



  template <class DERIVED>
  inline double squareNorm2(const BaseVector<DERIVED> & vec){

    const DERIVED & v=vec.getCVR();

    SquareNorm2Functor<DERIVED> sf(v);
    my_tbb::parallel_reduce(my_tbb::blocked_range<size_t>(0,v.flatSize(),10), sf );
    return sf.sum_;
  }

  typedef float * FloatPtr;
  typedef double * DoublePtr;

  // inline void swap (FloatPtr & a, FloatPtr & b){
  //   FloatPtr temp=a;
  //   a=b;
  //   b=temp;
  // }

  // inline void swap (DoublePtr & a, DoublePtr & b){
  //   DoublePtr temp=a;
  //   a=b;
  //   b=temp;
  // }


  // template <class T>
  // inline void swap (T & a, T & b){
  //   T temp=a;
  //   a=b;
  //   b=temp;
  // }

  template <class SCALAR_TYPE, int LEVEL>
  inline void swap(RectangularMultiVector<SCALAR_TYPE,LEVEL> & right,
		   RectangularMultiVector<SCALAR_TYPE,LEVEL> & left){
    if (right.owner()&&left.owner()){
      //      Legolas::swap(right.realDataPtr(),left.realDataPtr());
      {
	auto temp=right.realDataPtr();
	right.realDataPtr()=left.realDataPtr();
	left.realDataPtr()=temp;
      }
      {
	auto temp=right.dataPtr();
	right.dataPtr()=left.dataPtr();
	left.dataPtr()=temp;
      }

      //      Legolas::swap(right.dataPtr(),left.dataPtr());
    }
    else{
      INFOS("Legolas::swap is only possible for root vectors");
      throw std::runtime_error("Legolas::swap is only possible for root vectors");
    }

    return;
  }





}
#endif
