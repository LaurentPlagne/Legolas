#ifndef __LEGOLAS_FLATMULTIVECTOR_HXX__
#define __LEGOLAS_FLATMULTIVECTOR_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/Common/BasicTypes.hxx"
#include "Legolas/Vector/Assign/AssignMode.hxx"
#include "Legolas/Vector/Assign/Assign.hxx"
#include "Legolas/Vector/Expression/MatrixVectorExpression.hxx"
#include "Legolas/Vector/Expression/VectorExpression.hxx"
#include "Legolas/Vector/Expression/VectorOperations.hxx"
#include "Legolas/Vector/Shape/ShapeTraits.hxx"
#include "stringConvert.hxx"
// #include "X86Timer.hxx"

//#if USING_TBB == 1
//#include "tbb/parallel_reduce.h"
//#endif

#include "Legolas/Vector/FlatRecursiveTraits.hxx"
#include "Legolas/Vector/BuildLevelSizes.hxx"

namespace Legolas{


  template <class REAL_TYPE, int LEVEL>
  class FlatMultiVector : public BaseVector< FlatMultiVector<REAL_TYPE,LEVEL> >{
  public:

    typedef FlatRecursiveTraits<FlatMultiVector,
				FlatMultiVector<REAL_TYPE,LEVEL-1>,
				REAL_TYPE,LEVEL>    FRT;

    typedef typename FRT::Element               Element;
    typedef REAL_TYPE                           RealType;
    typedef REAL_TYPE *                         RealPtr;

    typedef Element &                           GetElement;
    typedef Element *                           ElementPtr;
    typedef const Element &                     ConstGetElement;
    
    typedef typename ShapeTraits<LEVEL>::Type   Shape;
    typedef const FlatMultiVector &             StoreType;
    
  private:

    bool       owner_;
    Element *  elementPtr_;
    int        size_;
    
    Shape      shape_;
    int        flatSize_;
    RealType * realDataPtr_;

  public:

    const RealPtr & getRef( void ) const { return realDataPtr_ ;}


    RealPtr       & realDataPtr( void )       {return realDataPtr_;}
    const RealPtr & realDataPtr( void ) const {return realDataPtr_;}
    bool             & owner( void )       { return owner_ ;}
    const       bool & owner( void ) const { return owner_ ;}
    ElementPtr       & elementPtr( void )       {return elementPtr_;}
    const ElementPtr & elementPtr( void ) const {return elementPtr_;}

    Shape       & shape( void )       {return shape_;}
    Shape shape( void ) const {return shape_;}

    int        & flatSize( void )       {return flatSize_;}
    const int  & flatSize( void ) const {return flatSize_;}

    double sizeInMB( void ){ return this->flatSize()*sizeof(RealType)/(1024.*1024);}

    const int & size( void ) const{ return size_;}
    int & size( void ) { return size_;}




    FlatMultiVector( void ):owner_(false),
			    elementPtr_(0),
			    size_(0),
			    shape_(),
			    flatSize_(0),
			    realDataPtr_(0)
    {}
    
    FlatMultiVector(const Shape & shape):owner_(true),
					 elementPtr_(0),
					 size_(0),
					 shape_(),
					 flatSize_(0),
					 realDataPtr_(0)
    {
      initializeFromShape(shape,*this);
    }

   
    FlatMultiVector(std::string s):owner_(true),
				   elementPtr_(0),
				   size_(0),
				   shape_(),
				   flatSize_(0),
				   realDataPtr_(0)
    {
      Shape shape(s);
      initializeFromShape(shape,*this);
    }

    
    //Constructor from a Vector Expression
    template <class DERIVED>
    FlatMultiVector(const BaseVector<DERIVED> & right):owner_(true),
						       elementPtr_(0),
						       size_(0),
						       shape_(),
						       flatSize_(0),
						       realDataPtr_(0)
    {
      const DERIVED & r=right.getCVR();
      initializeFromShape(FRT::buildShapeFromBaseVector(r),*this);
      (*this)=r;
    }  



    static inline void initializeFromShape(const Shape & shape, FlatMultiVector & v){

      //      INFOS("shape="<<shape);

      int elementSize=FRT::size(shape);
      if (elementSize!=0){
      v.owner_=true;
      v.elementPtr_=recursiveInitialize(buildLevelSizes(shape));
      
      //      v.size_=shape.size();
      v.size_=elementSize;

      //      INFOS("getRealDataPtr(this)="<<getRealDataPtr(v));

      const size_t flatSize=shape.flatSize();
      //      int flatSize=Legolas::getFlatSize(shape);

      //      INFOS("flatSize="<<flatSize);
      
      if (flatSize!=0){
	v.setRealDataPtr(shape,getRealDataPtr(v),flatSize);
      }

      //Initialize with zero
      //      v=0.0;
      }
    }

    void reshape(const Shape & shape){
      if (shape_!=shape){
	
	if (!elementPtr_) owner_=true;

	if (owner_){	//	owner_=true;      
	  delete[] elementPtr_;
	  initializeFromShape(shape,*this);
	}
	else{	//	owner_=false;
	  
	  INFOS("shape="<<shape_);
	  INFOS("source.shape_="<<shape);
	  INFOS("No reshape is allowed");
	  throw std::runtime_error("No reshape is allowed");
	}
      }
    }

    void resize(const Shape & shape){
      this->reshape(shape);
    }

    
    FlatMultiVector(const FlatMultiVector & source):owner_(true),
						    elementPtr_(0),
						    size_(0),
						    shape_(),
						    flatSize_(0),
						    realDataPtr_(0)
    {
      initializeFromShape(source.shape_,*this);
      (*this)=source;
    }
    
    FlatMultiVector & operator=(const FlatMultiVector & source){
      if (this!=&source){
	if (shape_!=source.shape_){
	  this->reshape(source.shape_);	
	}
	//	assert(this->shape_==source.shape_);
	//for (int i=0 ; i<flatSize_; i++) realDataPtr_[i]=source.realDataPtr_[i];
	Assign<EqualAssign,RealType>::apply(flatSize_,source.realDataPtr_,realDataPtr_);

      }
      return *this;
    }
      
    
    static inline Element * recursiveInitialize(const std::vector< std::vector<int> > & levelSizes){
      return FRT::recursiveInitialize(levelSizes);
    }


    static inline RealType * getRealDataPtr(const FlatMultiVector & vec){
      return FRT::getRealDataPtr(vec);
    }

    void setRealDataPtr(const Shape & shape, RealType * realDataPtr,int flatSize){
      FRT::setRealDataPtr(shape,realDataPtr,flatSize,*this);
    }


    ~FlatMultiVector( void ){
      if (owner_) delete[] elementPtr_;
    }

    

    Element & operator[](int i){ return elementPtr_[i];}
    const Element & operator[](int i)const{ return elementPtr_[i];}

    const RealType & operator()(int i)const{ return this->realDataPtr()[i];}


    
    bool operator == (const FlatMultiVector & r) const {
      bool isEqual=( (owner_==r.owner_) && 
		     (shape_==r.shape_) &&
		     (flatSize_==r.flatSize_) &&
		     (size_==r.size_) );
      if (isEqual && elementPtr_){
	for (int i=0 ; i< this->size() ; i++){
	  isEqual=(isEqual && (elementPtr_[i]==r.elementPtr_[i]));
	}
      }
      
      return isEqual;
    }

    bool operator != (const FlatMultiVector & r) const {
      return !((*this)==r);
    }   


    //Vectorial Methods

       /* ______________________ Common vectorial methods ___________________________  */

    class RealValueAdapter{
    private:
      RealType value_;
    public:
      RealValueAdapter(const RealType & value):value_(value){}
      inline RealType operator[](int i)const{return value_;}
    };

   
    class AssignValueFunctor{
    private:
      RealType alpha_;
      RealType * tPtr_;

    public:
      AssignValueFunctor(const RealType & alpha,RealType * t):alpha_(alpha),
						      tPtr_(t){}

      inline void operator() (int i) const { tPtr_[i]=alpha_;}
    };


    FlatMultiVector & operator = (double value){
      RealValueAdapter v(value);
      Assign<EqualAssign,RealType>::apply(flatSize_,v,realDataPtr_);
      return *this;
    }

    FlatMultiVector & operator = (float value){
      RealValueAdapter v(value);
      Assign<EqualAssign,RealType>::apply(flatSize_,v,realDataPtr_);
      return *this;
    }

    class PlusAssignValueFunctor{
    private:
      const RealType alpha_;
      RealType * tPtr_;

    public:

      PlusAssignValueFunctor(RealType alpha,RealType * t):alpha_(alpha),
						      tPtr_(t){}
      
      inline void operator() (int i) const { tPtr_[i]+=alpha_;}
    };




   FlatMultiVector & operator += (double value){
     RealValueAdapter v(value);
     Assign<PlusAssign,RealType>::apply(flatSize_,v,realDataPtr_);
     return *this;
    }

    FlatMultiVector & operator += (float value){
      RealValueAdapter v(value);
      Assign<PlusAssign,RealType>::apply(flatSize_,v,realDataPtr_);
      return *this;
    }


    FlatMultiVector & operator -= (double value){
      RealValueAdapter v(value);
      Assign<MinusAssign,RealType>::apply(flatSize_,v,realDataPtr_);
      return *this;
    }

    FlatMultiVector & operator -= (float value){
      RealValueAdapter v(value);
      Assign<MinusAssign,RealType>::apply(flatSize_,v,realDataPtr_);
      return *this;
    }

    class MultAssignValueFunctor{
    private:
      const RealType alpha_;
      RealType * tPtr_;

    public:

      MultAssignValueFunctor(RealType alpha,RealType * t):alpha_(alpha),
						      tPtr_(t){}
      
      inline void operator() (int i) const { tPtr_[i]*=alpha_;}

      inline RealType operator[] (int i) const { return tPtr_[i]*alpha_;}
    };

   class AxpyFunctor{
    private:
      const RealType alpha_;
      const RealType * sPtr_;
      RealType * tPtr_;

    public:
      AxpyFunctor(RealType alpha,RealType * s,RealType * t):alpha_(alpha),
							       sPtr_(s),
							       tPtr_(t){}

      inline void operator() (int i) const { tPtr_[i]+=alpha_*sPtr_[i];}

      inline RealType operator[] (int i) const { return alpha_*sPtr_[i];}
    };

    FlatMultiVector & operator *= (double value){
      Assign<EqualAssign,RealType>::apply(flatSize_,MultAssignValueFunctor(value,realDataPtr_),realDataPtr_);
      return *this;
    }

    FlatMultiVector & operator *= (float value){
      Assign<EqualAssign,RealType>::apply(flatSize_,MultAssignValueFunctor(value,realDataPtr_),realDataPtr_);
      return *this;
    }


    template <class OTHER_REAL_TYPE>
    void plusAssignVSX( const VectorScalarExpression<FlatMultiVector,OTHER_REAL_TYPE> & right, const RealType & fmult){
      
      assert(right.flatSize()<=flatSize_);
      const RealType factor=fmult*right.getScalarOperand();
      const FlatMultiVector & source=right.getVectorOperand();

      Assign<PlusAssign,RealType>::apply(flatSize_,AxpyFunctor(factor,source.realDataPtr_,realDataPtr_),realDataPtr_);
      
    }


    void operator += ( const VectorScalarExpression<FlatMultiVector,double> & right){this->plusAssignVSX(right,1.0);}
    void operator += ( const VectorScalarExpression<FlatMultiVector,float> & right){this->plusAssignVSX(right,1.0);}
    void operator -= ( const VectorScalarExpression<FlatMultiVector,double> & right){ this->plusAssignVSX(right,-1.0);}
    void operator -= ( const VectorScalarExpression<FlatMultiVector,float> & right){ this->plusAssignVSX(right,-1.0);}

    template <class EXPR>
    class ExprAdapter{
    private:
      const EXPR & expr_;
    public:
      ExprAdapter(const EXPR & expr):expr_(expr){}
      inline RealType operator[](int i)const{return expr_(i);}
    };
      
    

    //Expression Template
    template <class DERIVED>
    FlatMultiVector & operator = (const BaseVector<DERIVED> & right){
      const DERIVED & r=right.getCVR();
      
      //      ASSERT(r.shape()=this->shape());
      ExprAdapter<DERIVED> expr(r);

      Assign<EqualAssign,RealType>::apply(flatSize_,expr,realDataPtr_);

      return *this;
    }

    template <class DERIVED>
    FlatMultiVector & operator += (const BaseVector<DERIVED> & right){
      const DERIVED & r=right.getCVR();

      ExprAdapter<DERIVED> expr(r);

      Assign<PlusAssign,RealType>::apply(flatSize_,expr,realDataPtr_);

      return *this;
    }

    template <class DERIVED>
    FlatMultiVector & operator -= (const BaseVector<DERIVED> & right){
      const DERIVED & r=right.getCVR();

      ExprAdapter<DERIVED> expr(r);

	Assign<MinusAssign,RealType>::apply(flatSize_,expr,realDataPtr_);

	return *this;
    }



    template<class LEFT, class OPERATOR, class RIGHT>
    void operator = (const MatrixVectorExpression<LEFT,OPERATOR,RIGHT> & expression){
      typedef typename OPERATOR::template Engine< EqualAssign > Operator;
      Operator::apply(expression.matrixOperand(),expression.vectorOperand(),(*this));
    }


    template<class LEFT, class OPERATOR, class RIGHT>
    void operator += (const MatrixVectorExpression<LEFT,OPERATOR,RIGHT> & expression){
      typedef typename OPERATOR:: template Engine<PlusAssign> Operator;
      Operator::apply(expression.matrixOperand(),expression.vectorOperand(),(*this));
    }

    template<class LEFT, class OPERATOR, class RIGHT>
    void operator -= (const MatrixVectorExpression<LEFT,OPERATOR,RIGHT> & expression){
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

    
    /* ______________________ Common vectorial methods ___________________________  */


    void display(std::ostream& os) const {
      std::string prefix;
      FRT::display(*this,os,prefix);
    }

    void display(std::ostream& os, const std::string & prefix) const {
      FRT::display(*this,os,prefix);
    }
  
    
  };

  
  template <class REAL_TYPE, int LEVEL> 
  std::ostream& operator<< ( std::ostream& os, const FlatMultiVector<REAL_TYPE,LEVEL> & v )
  {
    v.display(os);
    return os;
  }

//   template <class T>
//   inline void swap (T & a, T & b){
//     T temp=a;
//     a=b;
//     b=temp;
//     }
  
  
  template <class REAL_TYPE, int LEVEL>
  inline void swap(FlatMultiVector<REAL_TYPE,LEVEL> & right, 
		   FlatMultiVector<REAL_TYPE,LEVEL> & left){
    if (right.owner()&&left.owner()){
      INFOS("SWAP");
      Legolas::swap(right.elementPtr(),left.elementPtr());
      Legolas::swap(right.realDataPtr(),left.realDataPtr());
    }
    else{
      INFOS("Legolas::swap is only possible for root vectors");
      throw std::runtime_error("Legolas::swap is only possible for root vectors");
    }
    
    return;
  }

      





  
}
#endif




