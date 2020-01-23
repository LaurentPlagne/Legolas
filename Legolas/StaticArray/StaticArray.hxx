#pragma once
#include <array>
namespace Legolas {

//    template<class T, int S,
//    class StaticArrayExpression{
//
//    };
  template <class DERIVED_ARRAY>
  struct StaticBaseArray {

    DERIVED_ARRAY & getArrayRef( void ) {
      return static_cast<DERIVED_ARRAY &>(*this);
    }

    const DERIVED_ARRAY & getArrayRef( void ) const {
      return static_cast<const DERIVED_ARRAY &>(*this);
    }

    const DERIVED_ARRAY & getConstArrayRef( void ) const {
      return static_cast<const DERIVED_ARRAY &>(*this);
    }
  };





    template <class L, class R, int S,int I>
    struct RecursiveFunctions{
        static inline void copy(L & l , const R & r){
          l[I]=r[I];
          RecursiveFunctions<L,R,S,I+1>::load(l,r);
        }
//        static inline void store(const std::array<T,S> & a , L & l){
//          p[I]=lI];
//          RecursiveFunctions<T,S,I+1>::store(a,p);
//        }
//

    };

    template <class L, class R, int S>
    struct RecursiveFunctions<L,R,S,S>{
        static inline void copy(L & l , const R & r){}
//        static inline void store(std::array<T,S> & a , const  L p){}
    };

    template <class T>
    struct Broadcast{
        T val_;
        Broadcast(const T & val):val_(val){}
        inline const T & operator[](size_t i) const {return val_;}
    };






    template<class T, int S>
    class StaticArray : public StaticBaseArray< StaticArray<T,S> >{
//        class alignas(32) StaticArray : public StaticBaseArray< StaticArray<T,S> >{
        std::array<T,S> a_;
//        T a_[S];

    public:
        using ScalarType=T;
        using Element=T;
        using StoreType=const StaticArray<T,S> &;
        static constexpr int size=S;


        StaticArray(){}

        StaticArray(T value){
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) (*this)[i]=value;
        }


        StaticArray(const StaticArray & other){
//          T result[S];
//#pragma GCC ivdep
//#pragma clang loop vectorize(enable)
//          for (size_t i = 0; i < S; i++) result[i] = other[i];
//          std::array<T,S> result;
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
#pragma vector aligned
          for (size_t i = 0; i < S; i++) a_[i] = other[i];
//          a_=result;
        }


        template <class DERIVED>
        StaticArray(const StaticBaseArray<DERIVED> & rhs){
          const DERIVED & r=rhs.getArrayRef();
           T result[S];
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) result[i]=r[i];
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) a_[i]=result[i];
        }

        inline const T & operator[](size_t i) const { return a_[i]; }
        inline T & operator[](size_t i) { return a_[i]; }

        StaticArray & operator = (const StaticArray &other) {
          if (this!=&other) {
            this->a_=other.a_;
          }
          return *this;
        }

        template <class DERIVED>
        StaticArray & operator = (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) a_[i]=r[i];
//          std::array<T,S> result;
//          for (size_t i = 0; i < S; i++) result[i]=r[i];
//          a_=result;
          return *this;
        }


        template <class DERIVED>
        StaticArray & operator += (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
          using Element=typename DERIVED::Element;

//          const Element a0=a_[0]+r[0];
//          const Element a1=a_[1]+r[1];
//          const Element a2=a_[2]+r[2];
//          const Element a3=a_[3]+r[3];
//          const Element a4=a_[4]+r[4];
//          const Element a5=a_[5]+r[5];
//          const Element a6=a_[6]+r[6];
//          const Element a7=a_[7]+r[7];
//          a_[0]=a0;
//          a_[1]=a1;
//          a_[2]=a2;
//          a_[3]=a3;
//          a_[4]=a4;
//          a_[5]=a5;
//          a_[6]=a6;
//          a_[7]=a7;
//#pragma GCC ivdep
//#pragma clang loop vectorize(enable)
//          for (size_t i = 0; i < S; i++) a_[i]+=r[i];
//          T result[S];
          std::array<T,S> result;
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) result[i]=a_[i]+r[i];
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
#pragma ivdep
#pragma assume(a_ % 64 == 0)

          for (size_t i = 0; i < S; i++) a_[i]=result[i];
//          a_=result;
          return *this;
        }


        template <class DERIVED>
        StaticArray & operator -= (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
          T result[S];
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) result[i]=a_[i]-r[i];
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) a_[i]=result[i];
//          std::array<T,S> result(a_);
//          for (size_t i = 0; i < S; i++) result[i]-=r[i];
//          a_=result;
          return *this;
        }


        template <class DERIVED>
        StaticArray & operator *= (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
          T result[S];

#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) result[i]=a_[i]*r[i];
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) a_[i]=result[i];
          return *this;
        }

        template <class DERIVED>
        StaticArray & operator /= (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
          T result[S];

#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) result[i]=a_[i]/r[i];
#pragma GCC ivdep
#pragma clang loop vectorize(enable)
          for (size_t i = 0; i < S; i++) a_[i]=result[i];
          return *this;
        }


       /* void display(std::ostream &os) const {
          os << "{";
          for (int k = 0; k < S; k++) {
            os << (*this)[k];
            if (k != (S - 1)) os << ' ';
          }
          os << "}" << std::endl;
        }*/

    };


    template<class DERIVED>
    std::ostream &operator<<(std::ostream &os, const Legolas::StaticBaseArray<DERIVED> &a) {
      const DERIVED & d=a.getArrayRef();
      constexpr size_t S=DERIVED::size;
      os << "{";
          for (int k = 0; k < S; k++) {
            os << d[k];
            if (k != (S - 1)) os << ' ';
          }
      os << "}" << std::endl;
      return os;
    }

    template <class T>
    struct StaticArrayAdd{
        template <class LEFT, class RIGHT>
        static inline
        T apply(const LEFT & left, const RIGHT & right){
          return left+right;
        }
    };

    template <class T>
    struct StaticArrayMinus{
        template <class LEFT, class RIGHT>
        static inline
        T apply(const LEFT & left, const RIGHT & right){
          return left-right;
        }
    };

    template <class T>
    struct StaticArrayMult{
        template <class LEFT, class RIGHT>
        static inline
        T apply(const LEFT & left, const RIGHT & right){
          return left*right;
        }
    };

    template <class T>
    struct StaticArrayDiv{
        template <class LEFT, class RIGHT>
        static inline
        T apply(const LEFT & left, const RIGHT & right){
          return left/right;
        }
    };


    template <class DRIGHT,class OP,class DLEFT>
    struct StaticExpression : StaticBaseArray< StaticExpression<DRIGHT,OP,DLEFT> >{
        const typename DRIGHT::StoreType r_;
        const typename DLEFT::StoreType l_;
        using StoreType=StaticExpression<DRIGHT,OP,DLEFT>;
        StaticExpression(const DRIGHT &r,const DLEFT & l):r_(r),l_(l){}
        using Element=typename DRIGHT::Element;

        inline Element operator[](size_t i) const { return OP::apply(r_[i], l_[i]); }
    };


    template <class T,int S>
    static inline std::array<T,S> initarray(const T & val){
      std::array<T,S> result;
      for (int i=0 ; i<S ; i++) result[i]=val;
      return result;
    }


    template <class DERIVED,class OP>
    struct StaticScalarExpression : StaticBaseArray< StaticScalarExpression<DERIVED,OP> > {
        using Element=typename DERIVED::Element;
        using StoreType=StaticScalarExpression<DERIVED,OP>;
        static constexpr int S=DERIVED::size;
//        const std::array<Element,S> a_;

        const Element a_;
        const typename DERIVED::StoreType d_;
//        StaticScalarExpression(const DERIVED & d,const Element & a):a_(initarray<Element,S>(a)),d_(d){
        StaticScalarExpression(const DERIVED & d,const Element & a):a_(a),d_(d){
//          for (size_t i = 0; i < S; i++) a_[i]=a;
        }
//        inline Element operator[](size_t i) const { return OP::apply(d_[i], a_[i]); }
        inline Element operator[](size_t i) const { return OP::apply(d_[i], a_); }
    };


    template <class DERIVED>
    StaticScalarExpression<DERIVED,StaticArrayMult<typename DERIVED::Element> > operator*(typename DERIVED::Element val,const Legolas::StaticBaseArray<DERIVED> &a){
      const DERIVED & d=a.getArrayRef();
      return StaticScalarExpression<DERIVED,StaticArrayMult<typename DERIVED::Element> >(d,val);
    }


    template <class DERIVED>
    StaticScalarExpression<DERIVED,StaticArrayMult<typename DERIVED::Element> > operator*(const Legolas::StaticBaseArray<DERIVED> &a, typename DERIVED::Element val){
      const DERIVED & d=a.getArrayRef();
      return StaticScalarExpression<DERIVED,StaticArrayMult<typename DERIVED::Element> >(d,val);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayMult<typename DRIGHT::Element>,DLEFT> operator*(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayMult<typename DRIGHT::Element> ,DLEFT>(dr,dl);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayDiv<typename DRIGHT::Element>,DLEFT> operator/(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayDiv<typename DRIGHT::Element>,DLEFT>(dr,dl);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayAdd<typename DRIGHT::Element>,DLEFT> operator+(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayAdd<typename DRIGHT::Element>,DLEFT>(dr,dl);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayMinus<typename DRIGHT::Element>,DLEFT> operator-(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayMinus<typename DRIGHT::Element>,DLEFT>(dr,dl);
    }



}


