#include <cstdlib>
#include <cmath>
#include <iostream>
#include <chrono>
#include "UTILITES.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"
#include "Legolas/StaticArray/StaticArray.hxx"




template<class T, int S>
struct MySum {
    static inline std::string name() { return "a+=b";}

    static inline void reference(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      for (size_t i=0 ; i<n ; i++) {
        a[i] += b[i];
      }
    };
    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      using SA=Legolas::StaticArray<T,S>;
      SA * as=reinterpret_cast<SA*>(&a[0]);
      SA * bs=reinterpret_cast<SA*>(&b[0]);

      size_t npack=n/S;

      for (size_t ip = 0; ip < npack; ip ++) {
        as[ip]+=bs[ip];
      }

    }
    static double nops(size_t n){ return double(n);}
};

template<class T, int S>
struct Sum3 {
    static inline std::string name() { return "a+=b+c";}
    static inline void reference(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      for (size_t i=0 ; i<n ; i++ ){
        a[i] += b[i] + c[i];
      }
    };

    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      using SA=Legolas::StaticArray<T,S>;
      SA * as=reinterpret_cast<SA*>(&a[0]);
      SA * bs=reinterpret_cast<SA*>(&b[0]);
      SA * cs=reinterpret_cast<SA*>(&c[0]);

      size_t npack=n/S;

#pragma unroll
      for (size_t ip = 0; ip < npack; ip ++) {
        as[ip]+=bs[ip]+cs[ip];
      }

    }
    static double nops(size_t n){ return 2.*double(n);}
};

template<class T, int S>
struct Axpy1 {
    static inline std::string name() { return "Y+=X*a";}
    static inline void reference(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      const T val=3.0;
      for (size_t i=0 ; i<n ; i++ ){
        a[i] += b[i]*val ;
      }
    };

    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      using SA=Legolas::StaticArray<T,S>;
      SA * as=reinterpret_cast<SA*>(&a[0]);
      SA * bs=reinterpret_cast<SA*>(&b[0]);

      size_t npack=n/S;
      const T val=3.0;

#pragma clang loop vectorize(disable)
#pragma unroll
      for (size_t ip = 0; ip < npack; ip ++) {
        as[ip]+=bs[ip]*val;
      }

    }
    static double nops(size_t n){ return 2.*double(n);}
};

template<class T, int S>
struct Axpy2 {
    static inline std::string name() { return "Y+=a*X";}
    static inline void reference(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      const T val=3.0;
      for (size_t i=0 ; i<n ; i++ ){
        a[i] += val*b[i] ;
      }
    };

    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      using SA=Legolas::StaticArray<T,S>;
      SA * as=reinterpret_cast<SA*>(&a[0]);
      SA * bs=reinterpret_cast<SA*>(&b[0]);

      size_t npack=n/S;
      const T val=3.0;

#pragma clang loop vectorize(disable)
      for (size_t ip = 0; ip < npack; ip ++) {
        as[ip]+=val*bs[ip];
      }

    }
    static double nops(size_t n){ return 2.*double(n);}
};


template<class T, int S>
struct Triad {
    static inline std::string name() { return "Y=a*X+b*Y+c*Z";}
    static inline void reference(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      const T v1=3.0;
      const T v2=4.0;
      const T v3=5.0;
      for (size_t i=0 ; i<n ; i++ ){
//        std::cout << a[i]<<" "<<b[i]<<" "<<c[i]<<" "<<std::endl;
        a[i] = v1*b[i]+v2*a[i]+v3*c[i] ;
//        std::cout << a[i]<<" "<<b[i]<<" "<<c[i]<<" "<<std::endl;

      }
    };

    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      using SA=Legolas::StaticArray<T,S>;
      SA * as=reinterpret_cast<SA*>(&a[0]);
      const SA * bs=reinterpret_cast<const SA*>(&b[0]);
      const SA * cs=reinterpret_cast<const SA*>(&c[0]);

      size_t npack=n/S;
      const T v1=3.0;
      const T v2=4.0;
      const T v3=5.0;
      for (size_t ip = 0; ip < npack; ip ++) {

        as[ip] = v1*bs[ip]+v2*as[ip]+v3*cs[ip] ;
      }

    }
    static double nops(size_t n){ return 5.*double(n);}
};



template<template<class,int> class OP, class T, int S>
void bench_operation_single(std::vector<T> &a,
               std::vector<T> &b,
               std::vector<T> &c) {
  const size_t n = a.size();



  std::vector<T> aref(a);
  std::vector<T> bref(b);
  std::vector<T> cref(c);

  OP<T, S>::apply(a, b, c, n);
  OP<T, S>::reference(aref, bref, cref, n);

  double squarednorm=0.0;
  for (size_t i=0 ; i< n ; i++){
    const double res=(aref[i]-a[i]);
    squarednorm+=res*res;
  }
  const double residual=sqrt(squarednorm);
  if (residual/double(n)>1.e-6){
    std::cout << "residual="<<residual<<std::endl;
    for (size_t i = 0; i < 16; i++)
      std::cout << a[i] << " ";
    std::cout << std::endl;
    for (size_t i = 0; i < 16; i++)
      std::cout << aref[i] << " ";
    std::cout << std::endl;
    exit(1);
  }


  const size_t ntrials = 1000;

  using clock=std::chrono::high_resolution_clock;

  std::chrono::time_point<clock> start_time, end_time;

  double min_time_ns=std::numeric_limits<double>::max();

  for (size_t trial = 0; trial < ntrials; trial++) {

    for (size_t i=0 ; i<a.size() ; i++) a[i]=T(i);
    for (size_t i=0 ; i<a.size() ; i++) b[i]=T(i);
    for (size_t i=0 ; i<a.size() ; i++) b[i]=T(i);

    start_time = clock::now();
    OP<T, S>::apply(a, b, c, n);
    end_time = clock::now();

    const double nanosec=std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    if (min_time_ns>nanosec)
    {
      min_time_ns = nanosec;
    }
  }
  const double gflops = OP<T, S>::nops(n) / (min_time_ns);
//  for (size_t i = 0; i < 16; i++)
//    std::cout << a[i] << " ";
//  std::cout << "a[" <<i<<"]="<<a[i]<<",";
//  std::cout << std::endl;
  std::cout << OP<T, S>::name()<<" S="<<S<<" : GFlops=" << gflops << " (residual="<<residual<<")"<<std::endl;
  return;

}

template<template<class,int> class OP, class T, int S>
void bench_operation(std::vector<T> &a,
               std::vector<T> &b,
               std::vector<T> &c) {

  bench_operation_single<OP,T,S>(a,b,c);
  bench_operation_single<OP,T,S>(a,b,c);
  bench_operation_single<OP,T,S>(a,b,c);

  for (size_t i = 0; i < 16; i++)
    std::cout << a[i] << " ";
  std::cout << std::endl;
}




int main( int argc,  char *argv[] )
{
  INFOS("MultiThomasTest");

  using RealType=float;
  const size_t n = 2 << 8;
  std::cout << "n=" << n <<std::endl;

  struct StaticWrap{
      double s_[4];
  };

  StaticWrap wa;
  std::cout << sizeof(wa) <<std::endl;



  std::vector<RealType > a(n);



  for (size_t i=0 ; i<a.size() ; i++) a[i]=RealType (i);
  std::vector<RealType > b(a);
  std::vector<RealType > c(a);

  StaticWrap * was=reinterpret_cast<StaticWrap*>(&a[0]);

  std::cout << was[0].s_[0] << std::endl;
  std::cout << was[1].s_[0] << std::endl;

//  using ThisBench=ThomasBench<Legolas::Array<RealType,2,8,2>,SeqMap>;
//
//  const double gflops=ThisBench ::evalGflops(64);
//
//  std::cout << ThisBench::name() << " gflops="<<gflops << std::endl;

  bench_operation<Sum3,RealType ,16>(a,b,c);

  bench_operation<Axpy1,RealType ,16>(a,b,c);
  bench_operation<Axpy2,RealType ,16>(a,b,c);


  return 0;
}




