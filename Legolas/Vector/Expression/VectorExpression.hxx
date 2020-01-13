#ifndef __LEGOLAS_VECTOREXPRESSION_HXX__
#define __LEGOLAS_VECTOREXPRESSION_HXX__

#include <assert.h>
#include "UTILITES.hxx"
#include "Legolas/EigenInterface.hxx"
#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/Common/BasicTypes.hxx"
#include "Legolas/Vector/Expression/ScalarTraits.hxx"

#include "Legolas/Vector/Expression/VectorExpressionTraits.hxx"

namespace Legolas{

  template <class LEFT, class BINARY_OP, class RIGHT>
  class VectorExpression : public BaseVector< VectorExpression<LEFT,BINARY_OP,RIGHT> >
  {
  public:

    typedef typename RIGHT::RealType RealType;
    typedef typename RIGHT::Shape    Shape;

    //    typedef const VectorExpression &    StoreType;
    typedef VectorExpression    StoreType;
    //    typedef typename RIGHT::Element     Element;
    typedef BaseVector<LEFT>            GVL;
    typedef BaseVector<RIGHT>           GVR;

    VectorExpression(const GVL  & left, const GVR & right):left_(left.getCVR()),
							   right_(right.getCVR())

    {
      assert(left.getCVR().shape()==right.getCVR().shape());
      MESSAGE("Vector Expression Ctor");
    }


    VectorExpression(const LEFT & left, const RIGHT & right):left_(left),
							     right_(right)

    {
      assert(left.shape()==right.shape());
      MESSAGE("Vector Expression Ctor");
    }


    typedef VectorExpression<typename LEFT::Element,BINARY_OP,typename RIGHT::Element> SubExpression;
    typedef BinaryVectorExpressionTraits<SubExpression,BINARY_OP,typename RIGHT::Element> TT;
    typedef typename TT::ReturnType ReturnType;
    typedef ReturnType Element;


    inline ReturnType operator[]( SizeType i) const {
      //      //      return BINARY_OP::template Engine<Element>::apply(left_[i],right_[i]);
      return TT::getElement(left_,right_,i);
    }


    inline RealType operator()(SizeType i) const {
      return BINARY_OP::template Engine<RealType>::apply(left_(i),right_(i));
    }


    //    inline SizeType flatSize( void ) const { return right_.flatSize();}
    //    inline SizeType size( void ) const { return right_.size();}

    inline size_t flatSize( void ) const { return right_.flatSize();}
    inline size_t size( void ) const { return right_.size();}
    inline Shape shape( void ) const {return right_.shape() ; }

    bool isAllocated( void ) const {return (left_.isAllocated() || right_.isAllocated()) ;}

    inline  typename LEFT::StoreType left( void ) const { return left_ ; }
    inline  typename RIGHT::StoreType right( void ) const { return right_ ; }

#ifdef LEGOLAS_USE_EIGEN

    inline auto eigenVectorView( void ) const -> decltype(BINARY_OP::autoApply(this->left().eigenVectorView(),
									       this->right().eigenVectorView())){
      return BINARY_OP::autoApply(left_.eigenVectorView(),right_.eigenVectorView());
    }


    inline auto eigenVectorView(size_t begin, size_t length) const -> decltype(BINARY_OP::autoApply(this->left().eigenVectorView(begin,length),
												    this->right().eigenVectorView(begin,length))){
      return BINARY_OP::autoApply(left_.eigenVectorView(begin,length),right_.eigenVectorView(begin,length));
    }

#endif



  private:

    typename LEFT::StoreType  left_;
    typename RIGHT::StoreType right_;


  };



  //////////////////////////////////////////////////////////////////////////////////////

  template <class VECTOR, class SCALAR>
  class VectorScalarExpression : public BaseVector< VectorScalarExpression < VECTOR,SCALAR > >
  {
  public:

    typedef typename VECTOR::RealType RealType;
    typedef typename VECTOR::Shape     Shape;

    typedef SCALAR                                           Scalar;
    typedef VectorScalarExpression                           StoreType;
    typedef BaseVector<VECTOR>                               GV;

    typedef VectorScalarExpression<typename VECTOR::Element,Scalar> SubExpression;
    typedef UnaryVectorExpressionTraits<SubExpression,typename VECTOR::Element> UVET;
    typedef typename UVET::ReturnType ReturnType;
    typedef ReturnType Element;

    typedef typename ScalarTraits<SCALAR,typename VECTOR::Element>::Type      ScalarStoreType;


    VectorScalarExpression(const GV & v, const Scalar & a):a_(a),
							   v_(v.getCVR())

    {
      MESSAGE("Vector Scalar Expression Ctor");
    }


    VectorScalarExpression(const VECTOR & v, const Scalar & a):a_(a),
							   v_(v.getCVR())

    {
      MESSAGE("Vector Scalar Expression Ctor");
    }




    inline ReturnType operator[]( SizeType i) const {
      return UVET::getElement(a_,v_,i);
    }

    inline RealType operator()(SizeType i) const {
      return a_*v_(i);
    }


    //    inline SizeType flatSize( void ) const { return v_.flatSize();}
    //    inline SizeType size( void ) const { return v_.size();}

    inline size_t flatSize( void ) const { return v_.flatSize();}
    inline size_t size( void ) const { return v_.size();}
    inline Shape shape( void ) const {return v_.shape() ; }

    inline const ScalarStoreType & getScalarOperand( void ) const { return a_ ;}
    inline typename VECTOR::StoreType getVectorOperand( void ) const { return v_ ;}

    bool isNull( void ) const { return v_.isNull() ;}

    bool isAllocated( void ) const {return v_.isAllocated() ;}



    //    inline const

    inline ScalarStoreType scalarValue( void ) const { return a_;}
    inline typename VECTOR::StoreType vectorExpression( void ) const { return v_;}

#ifdef LEGOLAS_USE_EIGEN

    inline auto eigenVectorView( void ) const -> decltype(this->scalarValue()*this->vectorExpression().eigenVectorView()) {
      return this->scalarValue()*this->vectorExpression().eigenVectorView();
    }

    inline auto eigenVectorView(size_t begin, size_t length) const -> decltype(this->scalarValue()*this->vectorExpression().eigenVectorView(begin,length)) {
      return this->scalarValue()*this->vectorExpression().eigenVectorView(begin,length);
    }
#endif

  private:

    ScalarStoreType  a_;
    typename VECTOR::StoreType v_;

  };



  //////////////////////////////////////////////////////////////////////////////////////

  template <class FUNCTOR, class VECTOR>
  class VectorMapExpression : public BaseVector< VectorMapExpression < FUNCTOR,VECTOR> >
  {
  public:

    typedef FUNCTOR                          Functor;
    typedef VectorMapExpression              StoreType;
    typedef BaseVector<VECTOR>               GV;
    typedef typename VECTOR::RealType        RealType;
    typedef typename VECTOR::Shape           Shape;

    struct ShouldNotBeUsed{};
    typedef ShouldNotBeUsed Element;

  //   typedef VectorMapExpression<FUNCTOR,typename VECTOR::Element> SubExpression;
//     typedef VectorMapSubExpressionTraits<SubExpression,typename VECTOR::Element> VMSET;
//     typedef typename VMSET::ReturnType ReturnType;
//    typedef ReturnType Element;


    VectorMapExpression(const FUNCTOR & f, const GV & v):f_(f),
							 v_(v.getCVR())

    {
      MESSAGE("Vector Map Expression Ctor");
    }


//     inline ReturnType operator[]( SizeType i) const {
//       return VMSET::map(f_,v_[i]);
//       //      return UVET::getElement(f_,v_,i);
//     }

    inline RealType operator()(SizeType i) const {
      return f_(v_(i));
    }


    inline SizeType flatSize( void ) const { return v_.flatSize();}
    inline SizeType size( void ) const { return v_.size();}
    inline Shape shape( void ) const {return v_.shape() ; }

    //    inline const ScalarStoreType & getScalarOperand( void ) const { return a_ ;}
    //    inline typename VECTOR::StoreType getVectorOperand( void ) const { return v_ ;}

    bool isNull( void ) const { return v_.isNull() ;}

    bool isAllocated( void ) const {return v_.isAllocated() ;}


  private:

    const FUNCTOR &  f_;
    typename VECTOR::StoreType v_;

  };


  // template <class LEFT, class BINARY_OP, class RIGHT>
  // inline auto eigenVectorView( const VectorExpression<LEFT,BINARY_OP,RIGHT> & cve ) -> decltype(BINARY_OP::autoApply(eigenVectorView(cve.left()),
  // 														     eigenVectorView(cve.right())))
  // {
  //   return BINARY_OP::autoApply(eigenVectorView(cve.left()),eigenVectorView(cve.right()));
  // }

}

#endif
