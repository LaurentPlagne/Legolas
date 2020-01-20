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
        std::array<T,S> a_;
//        T a_[S];

    public:
        using ScalarType=T;
        using Element=T;
        static constexpr int size=S;


        StaticArray(){}

        StaticArray(T value){
          for (size_t i = 0; i < S; i++) (*this)[i]=value;
        }


        StaticArray(const StaticArray & other){
          std::array<T,S> result;
          for (size_t i = 0; i < S; i++) result[i] = other[i];
          a_=result;
        }


        template <class DERIVED>
        StaticArray(const StaticBaseArray<DERIVED> & rhs){
          const DERIVED & r=rhs.getArrayRef();
          for (size_t i = 0; i < S; i++) a_[i]=r[i];
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
//          for (size_t i = 0; i < S; i++) a_[i]=r[i];

          std::array<T,S> result;
          for (size_t i = 0; i < S; i++) result[i]=r[i];
          a_=result;
          return *this;
        }


        template <class DERIVED>
        StaticArray & operator += (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
//          for (size_t i = 0; i < S; i++) a_[i]+=r[i];
          std::array<T,S> result(a_);
          for (size_t i = 0; i < S; i++) result[i]+=r[i];
          a_=result;
          return *this;
        }


        template <class DERIVED>
        StaticArray & operator -= (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
//          for (size_t i = 0; i < S; i++) a_[i]-=r[i];
          std::array<T,S> result(a_);
          for (size_t i = 0; i < S; i++) result[i]-=r[i];
          a_=result;
          return *this;
        }


        template <class DERIVED>
        StaticArray & operator *= (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
          for (size_t i = 0; i < S; i++) a_[i]*=r[i];
          return *this;
        }

        template <class DERIVED>
        StaticArray & operator /= (const StaticBaseArray<DERIVED> & rhs) {
          const DERIVED & r=rhs.getArrayRef();
          for (size_t i = 0; i < S; i++) a_[i]/=r[i];
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

    struct StaticArrayAdd{
        template <class LEFT, class RIGHT>
        static inline
        auto apply(const LEFT & left, const RIGHT & right)-> decltype(left+right){
          return left+right;
        }
    };

    struct StaticArrayMinus{
        template <class LEFT, class RIGHT>
        static inline
        auto apply(const LEFT & left, const RIGHT & right) -> decltype(left-right){
          return left-right;
        }
    };

    struct StaticArrayMult{
        template <class LEFT, class RIGHT>
        static inline
        auto apply(const LEFT & left, const RIGHT & right)  -> decltype(left*right){
          return left*right;
        }
    };

    struct StaticArrayDiv{
        template <class LEFT, class RIGHT>
        static inline
        auto apply(const LEFT & left, const RIGHT & right)  -> decltype(left/right){
          return left/right;
        }
    };


    template <class DRIGHT,class OP,class DLEFT>
    struct StaticExpression : StaticBaseArray< StaticExpression<DRIGHT,OP,DLEFT> >{
        const DRIGHT &r_;
        const DLEFT &l_;
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
        const DERIVED &d_;
        using Element=typename DERIVED::Element;
        static constexpr int S=DERIVED::size;
        const std::array<Element,S> a_;
//        const Element a_;
        StaticScalarExpression(const DERIVED & d,const Element & a):d_(d),a_(initarray<Element,S>(a)){
//          for (size_t i = 0; i < S; i++) a_[i]=a;
        }
        inline Element operator[](size_t i) const { return OP::apply(d_[i], a_[i]); }
    };


    template <class DERIVED>
    StaticScalarExpression<DERIVED,StaticArrayMult> operator*(typename DERIVED::Element val,const Legolas::StaticBaseArray<DERIVED> &a){
      const DERIVED & d=a.getArrayRef();
      return StaticScalarExpression<DERIVED,StaticArrayMult>(d,val);
    }


    template <class DERIVED>
    StaticScalarExpression<DERIVED,StaticArrayMult> operator*(const Legolas::StaticBaseArray<DERIVED> &a, typename DERIVED::Element val){
      const DERIVED & d=a.getArrayRef();
      return StaticScalarExpression<DERIVED,StaticArrayMult>(d,val);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayMult,DLEFT> operator*(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayMult,DLEFT>(dr,dl);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayDiv,DLEFT> operator/(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayDiv,DLEFT>(dr,dl);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayAdd,DLEFT> operator+(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayAdd,DLEFT>(dr,dl);
    }

    template <class DRIGHT,class DLEFT>
    StaticExpression<DRIGHT,StaticArrayMinus,DLEFT> operator-(const Legolas::StaticBaseArray<DRIGHT> &r,
                                                             const Legolas::StaticBaseArray<DLEFT> &l){
      const DRIGHT & dr=r.getArrayRef();
      const DLEFT & dl=l.getArrayRef();
      return StaticExpression<DRIGHT,StaticArrayMinus,DLEFT>(dr,dl);
    }



}


