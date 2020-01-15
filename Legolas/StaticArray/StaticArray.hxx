#pragma once

namespace Legolas {

//    template<class T, int S,
//    class StaticArrayExpression{
//
//    };


    template <class T, int S,int I>
    struct RecursiveFunctions{
        static inline void load(std::array<T,S> & a , T * p){
          a[I]=p[I];
          RecursiveFunctions<T,S,I+1>::load(a,p);
        }
        static inline void store(std::array<T,S> & a , T * p){
          p[I]=a[I];
          RecursiveFunctions<T,S,I+1>::store(a,p);
        }
    };

    template <class T, int S>
    struct RecursiveFunctions<T,S,S>{
        static inline void load(std::array<T,S> & a , T * p){}
        static inline void store(std::array<T,S> & a , T * p){}
    };

    template<class T, int S>
    class StaticArray {
//        std::array<T,S> a_;
        T a_[S];

    public:


        StaticArray(const StaticArray & other){
          for (size_t i = 0; i < S; i++) (*this)[i] = other[i];
        }

        const T & operator[](size_t i) const { return a_[i]; }

        inline T & operator[](size_t i) { return a_[i]; }

        StaticArray & operator = (const StaticArray &other) {
//          a_=other.a_;
//          this->store();
          for (size_t i = 0; i < S; i++) (*this)[i] = other[i];
        }


        StaticArray operator + (const StaticArray & other) const {
          StaticArray result(other);
          result+=other;
          return result;
        }

#define NOALIAS_WARANTY
        StaticArray &operator+=(const StaticArray & other) {
#ifdef NOALIAS_WARANTY
          std::array<T,S> result;
          for (size_t i = 0; i < S; i++) result[i] = a_[i]+other.a_[i];
          for (size_t i = 0; i < S; i++) this->a_[i] = result[i];
#else
          for (size_t i = 0; i < S; i++) this->a_[i] += other.a_[i];
#endif


          return *this;
        }

        StaticArray &operator-=(const StaticArray &other) {
          for (size_t i = 0; i < S; i++) (*this)[i] -= other[i];
        }

        StaticArray &operator*=(const StaticArray &other) {
          for (size_t i = 0; i < S; i++) (*this)[i] *= other[i];
        }

        StaticArray &operator/=(const StaticArray &other) {
          for (size_t i = 0; i < S; i++) (*this)[i] /= other[i];
        }

        void display(std::ostream &os) const {
          os << "{";
          for (int k = 0; k < S; k++) {
            os << (*this)[k];
            if (k != (S - 1)) os << ' ';
          }
          os << "}" << std::endl;
        }

    };


    template<class T, int S>
    std::ostream &operator<<(std::ostream &os, const Legolas::StaticArray<T,S> &a) {
      a.display(os);
      return os;
    }

}


