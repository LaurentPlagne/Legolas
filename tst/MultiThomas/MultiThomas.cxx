#include <cstdlib>
#include <cmath>
#include <iostream>
#include <chrono>
#include "UTILITES.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"
#include "helper.hxx"
#include "Legolas/StaticArray/StaticArray.hxx"


struct ThomasSolver{
  template <class A2D>
  void operator()(int begin, int end,
                  A2D D2D, A2D U2D, A2D L2D,
                  A2D B2D, A2D X2D) const {
    typedef typename A2D::Element Element;
    typedef typename A2D::RealType  Scalar;
    Element S(X2D[0].shape());
    Scalar  one(1.0),s,sm1;

    for (int j=begin ; j<end ; j++){
      auto D=D2D[j];
      auto U=U2D[j]; auto L=L2D[j];
      auto B=B2D[j]; auto X=X2D[j];
      s=D[0];
      sm1=one/s;
      const int size=X.size();
      //forward
      X[0]=B[0]*sm1;
      for(int i=1; i<size; i++){
        S[i]=U[i-1]*sm1;
        s=D[i]-L[i]*S[i];
        X[i]=B[i]-L[i]*X[i-1];
        sm1=one/s;
        X[i]*=sm1;
      }
      //backward
      for (int i=(size-2);i>=0 ; i--){
        X[i]-=S[i+1]*X[i+1];
      }
    }
  }
};


struct ThomasSolverInPlace{
  template <class A2D>
  void operator()(int begin, int end,
                  A2D D2D, A2D U2D, A2D L2D, A2D X2D) const {
    typedef typename A2D::Element Element;
    typedef typename A2D::RealType  Scalar;
    Element S(X2D[0].shape());
    Scalar  one(1.0),s,sm1;

    for (int j=begin ; j<end ; j++){
      auto D=D2D[j];
      auto U=U2D[j]; auto L=L2D[j];
      auto X=X2D[j];
      s=D[0];
      sm1=one/s;
      const int size=X.size();
      //forward
      X[0]*=sm1;
      for(int i=1; i<size; i++){
        S[i]=U[i-1]*sm1;
        s=D[i]-L[i]*S[i];
        X[i]-=L[i]*X[i-1];
        sm1=one/s;
        X[i]*=sm1;
      }
      //backward
      for (int i=(size-2);i>=0 ; i--){
        X[i]-=S[i+1]*X[i+1];
      }
    }
  }
};


struct LaplacianSolver{
  template <class A2D>
  void operator()(int begin, int end,
                  A2D B2D, A2D X2D) const {
    typedef typename A2D::Element Element;
    typedef typename A2D::RealType  Scalar;
    Element S(X2D[0].shape());
    Scalar  one(1.0),s,sm1;
    Scalar minusOne(-1.0);
    Scalar two(2.0);

    for (int j=begin ; j<end ; j++){
      auto B=B2D[j]; auto X=X2D[j];
      s=two;
      sm1=one/s;

      const int size=X.size();
      //forward
      X[0]=B[0]*sm1;
      for(int i=1; i<size; i++){
        S[i]=minusOne*sm1;
        s=two+S[i];
        X[i]=B[i]+X[i-1];
        sm1=one/s;
        X[i]*=sm1;
      }
      //backward
      for (int i=(size-2);i>=0 ; i--){
        X[i]-=S[i+1]*X[i+1];
      }
    }
  }
};


struct SeqMap{
  template <typename... Args>
  static inline void apply(const Args... args){Legolas::map(args...);}
  static inline std::string name( void ){ return "sequential";}
};

struct ParMap{
  template <typename... Args>
  static inline void apply(const Args... args){Legolas::parmap(args...);}
  static inline std::string name( void ){ return "parallel";}
};


template <class MAP_TYPE>
struct TimeEval{

  template <typename... Args>
  static inline double apply(const Args... args){

    const int nloop=5;
    const int nsample=5;

    double minTime=std::numeric_limits<double>::max();
    for (int s=0 ; s<nsample ; s++){

      auto start_time = std::chrono::high_resolution_clock::now();
      for (int i=0 ; i<nloop ; i++){
        MAP_TYPE::apply(args...);
      }
      auto end_time = std::chrono::high_resolution_clock::now();

      const double microsec=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
      const double sec=microsec/1.e6;
      if (minTime>sec) minTime=sec;
    }
//    display2D(X2D);
    return minTime/double(nloop);
  }
};


template <class V2D, class MAP_TYPE>
struct ThomasLDLBench{

  static inline std::string name( void ){
    std::string result("ThomasLDLSolver");
    result+="_"+std::to_string(V2D::packSize)+"_"+MAP_TYPE::name();
    return result;
  }


  static inline double evalGflops(const int nx){
//    INFOS("nx="<<nx);
    const int ny=nx*nx;

    V2D X2D(ny,nx);
    V2D B2D(ny,nx);
    V2D D2D(ny,nx);
    V2D L2D(ny,nx);

    X2D.fill(1.0);
    B2D.fill(1.0);
    L2D.fill(-1.0);
    D2D.fill(2.0);

    const double t=TimeEval<MAP_TYPE>::apply(ThomasSolver(),D2D,L2D,L2D,B2D,X2D);
    display2D(X2D);
    return nbops(nx,ny)/(t*1.e9);

  }

  static inline double nbops(int nx, int ny){
    return 13.*nx*ny;
  }

};

template <class V2D, class MAP_TYPE>
struct ThomasBench{

  static inline std::string name( void ){
    std::string result("Thomas");
    result+="_"+std::to_string(V2D::packSize)+"_"+MAP_TYPE::name();
    return result;
  }


  static inline double evalGflops(const int nx){
//    INFOS("nx="<<nx);
    const int ny=nx*nx;

    V2D X2D(ny,nx);
    V2D B2D(ny,nx);
    V2D U2D(ny,nx);
    V2D D2D(ny,nx);
    V2D L2D(ny,nx);

    X2D.fill(1.0);
    B2D.fill(1.0);
    U2D.fill(-1.0);
    L2D.fill(-1.0);
    D2D.fill(2.0);

    const double t=TimeEval<MAP_TYPE>::apply(ThomasSolver(),D2D,U2D,L2D,B2D,X2D);
    display2D(X2D);
    return nbops(nx,ny)/(t*1.e9);

  }

  static inline double nbops(int nx, int ny){
    return 13.*nx*ny;
  }

};

template <class V2D, class MAP_TYPE>
struct LaplacianBench{

  static inline std::string name( void ){
    std::string result("Laplacian");
    result+="_"+std::to_string(V2D::packSize)+"_"+MAP_TYPE::name();
    return result;
  }


  static inline double evalGflops(const int nx){
//    INFOS("nx="<<nx);
    const int ny=nx*nx;

    V2D X2D(ny,nx);
    V2D B2D(ny,nx);

    X2D.fill(1.0);
    B2D.fill(1.0);

    const double t=TimeEval<MAP_TYPE>::apply(LaplacianSolver(),B2D,X2D);
    display2D(X2D);
    return nbops(nx,ny)/(t*1.e9);

  }

  static inline double nbops(int nx, int ny){
    return 11.*nx*ny;
  }

};

template <class AlgoBench>
void goBench(void){
  std::vector<int> sizes=makeSizes();
  const int nbPoints=sizes.size();
  std::vector<double> perfs(nbPoints);
  for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=AlgoBench::evalGflops(sizes[i]);
  std::string filename(AlgoBench::name());
  filename+=".dat";
  savePerf(sizes,perfs,filename);
}


template <class T, int S>
void test_sum(std::vector<T> & a,
              std::vector<T> & b,
              std::vector<T> & c){

  Legolas::StaticArray<T,S> ab1(&a[0]);
  Legolas::StaticArray<T,S> ab2(&b[0]);
  Legolas::StaticArray<T,S> ab3(&c[0]);

  std::cout << ab1 << std::endl;
  std::cout << ab2 << std::endl;
  std::cout << ab3 << std::endl;
  const int N=1000000000;
  auto start_time = std::chrono::high_resolution_clock::now();
  for (size_t i=0 ; i<N ; i++) ab3+=ab1;
  auto end_time = std::chrono::high_resolution_clock::now();
  const double microsec=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
  const double sec=microsec/1.e6;
  const double gflops=4.*double(N)/(sec*1.e9);
  std::cout << ab3 << std::endl;
  std::cout << "GFlops=" << gflops << std::endl;
  return ;

}



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
      SA * bs=reinterpret_cast<SA*>(&b[0]);
      SA * cs=reinterpret_cast<SA*>(&c[0]);

      size_t npack=n/S;
      const T v1=3.0;
      const T v2=4.0;
      const T v3=5.0;
      for (size_t ip = 0; ip < npack; ip ++) {
//        std::cout << as[ip]<<" "<<bs[ip]<<" "<<cs[ip]<<" "<<std::endl;
//
//        std::cout << "v1*bs[ip]="<<v1*bs[ip]<<std::endl;
//        std::cout << "v2*as[ip]="<<v2*as[ip]<<std::endl;
//        std::cout << "v1*bs[ip]+v2*as[ip]="<<v1*bs[ip]+v2*as[ip]<<std::endl;

        as[ip] = v1*bs[ip]+v2*as[ip]+v3*cs[ip] ;



//        std::cout << as[ip]<<" "<<bs[ip]<<" "<<cs[ip]<<" "<<std::endl;

      }

//      std::cout << "ici" << std::endl; exit(0);
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

  using RealType=double;
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


//  Legolas::StaticArray<RealType,4> sa(&a[0]);

//  std::cout << sa << std::endl;



//  test_sum<RealType,4>(a,b,c);


  bench_operation<MySum,RealType ,16>(a,b,c);

  bench_operation<Sum3,RealType ,8>(a,b,c);

  bench_operation<Axpy1,RealType ,8>(a,b,c);

  bench_operation<Axpy2,RealType ,8>(a,b,c);

  bench_operation<Triad,RealType ,16>(a,b,c);


  // goBench< ThomasLDLBench<Legolas::Array<RealType,2>, SeqMap > >();
  // goBench< ThomasLDLBench<Legolas::Array<RealType,2,4,2>, SeqMap > >();
  // goBench< ThomasLDLBench<Legolas::Array<RealType,2,8,2>, SeqMap > >();
  // goBench< ThomasLDLBench<Legolas::Array<RealType,2>, ParMap > >();
  // goBench< ThomasLDLBench<Legolas::Array<RealType,2,4,2>, ParMap > >();
  // goBench< ThomasLDLBench<Legolas::Array<RealType,2,8,2>, ParMap > >();

  // goBench< LaplacianBench<Legolas::Array<RealType,2>, SeqMap > >();
  // goBench< LaplacianBench<Legolas::Array<RealType,2,4,2>, SeqMap > >();
  // goBench< LaplacianBench<Legolas::Array<RealType,2,8,2>, SeqMap > >();
  // goBench< LaplacianBench<Legolas::Array<RealType,2>, ParMap > >();
  // goBench< LaplacianBench<Legolas::Array<RealType,2,4,2>, ParMap > >();
  // goBench< LaplacianBench<Legolas::Array<RealType,2,8,2>, ParMap > >();

  // goBench< ThomasBench<Legolas::Array<RealType,2>, SeqMap > >();
  // goBench< ThomasBench<Legolas::Array<RealType,2,4,2>, SeqMap > >();
  // goBench< ThomasBench<Legolas::Array<RealType,2,8,2>, SeqMap > >();
  // goBench< ThomasBench<Legolas::Array<RealType,2>, ParMap > >();
  // goBench< ThomasBench<Legolas::Array<RealType,2,4,2>, ParMap > >();
  // goBench< ThomasBench<Legolas::Array<RealType,2,8,2>, ParMap > >();
//  {
//    typedef Legolas::Array<RealType,2,8,2> A2D;
//    typedef SeqMap MapType;
//    typedef ThomasBench<A2D,MapType> AlgoBench;
//    for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=AlgoBench::evalGflops(sizes[i]);
//    std::string filename(AlgoBench::name());
//    filename+="_"+std::to_string(A2D::packSize)+"_"+MapType::name();
//    savePerf(sizes,perfs,filename);
//  }
  //    {
  //      typedef Legolas::Array<RealType,2,8,2> A2D;
  //      for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=executeP<A2D>(sizes[i]);
  //      savePerf(sizes,perfs,"A2D_82P.dat");
  //    }
  //    {
  //      typedef Legolas::Array<RealType,2,4,2> A2D;
  //      for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=execute<A2D>(sizes[i]);
  //      savePerf(sizes,perfs,"A2D_42.dat");
  //    }
  //    {
  //      typedef Legolas::Array<RealType,2,4,2> A2D;
  //      for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=executeP<A2D>(sizes[i]);
  //      savePerf(sizes,perfs,"A2D_42P.dat");
  //    }
  //    {
  //      typedef Legolas::Array<RealType,2> A2D;
  //      for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=execute<A2D>(sizes[i]);
  //      savePerf(sizes,perfs,"A2D.dat");
  //    }
  //    {
  //      typedef Legolas::Array<RealType,2,4,2> A2D;
  //      for (int i=nbPoints-1 ; i>=0 ; i--) perfs[i]=executeP<A2D>(sizes[i]);
  //      savePerf(sizes,perfs,"A2DP.dat");
  //    }

  return 0;
}




