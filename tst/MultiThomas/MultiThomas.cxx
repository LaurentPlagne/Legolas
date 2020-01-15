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
    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      using SA=Legolas::StaticArray<T,S>;
      SA * as=reinterpret_cast<SA*>(&a[0]);
      SA * bs=reinterpret_cast<SA*>(&b[0]);

//      StaticArray< * was=reinterpret_cast<StaticWrap*>(&a[0]);

//      Legolas::StaticArray<T,S> as(&a[0]),bs(&b[0]);
      for (size_t ip = 0; ip < n/S; ip ++) {
        as[ip]+=bs[ip];

      }
    }
};
template<class T>
struct MySum<T,4> {
    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
      for (size_t i = 0; i < n; i += 4) {
        const T r0 = b[i + 0]+a[i + 0];
        const T r1 = b[i + 1]+a[i + 1];
        const T r2 = b[i + 2]+a[i + 2];
        const T r3 = b[i + 3]+a[i + 3];
        a[i + 0] = r0;
        a[i + 1] = r1;
        a[i + 2] = r2;
        a[i + 3] = r3;
//            const T b0 = b[i + 0];
//            const T b1 = b[i + 1];
//            const T b2 = b[i + 2];
//            const T b3 = b[i + 3];
//            a[i + 0] += b0;
//            a[i + 1] += b1;
//            a[i + 2] += b2;
//            a[i + 3] += b3;
      }
    }
};

//template<class T>
//struct MySum<T,8> {
//    static inline void apply(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c, size_t n) {
//      for (size_t i = 0; i < n; i += 8) {
//        const T b0 = b[i + 0]+a[i + 0];
//        const T b1 = b[i + 1]+a[i + 1];
//        const T b2 = b[i + 2]+a[i + 2];
//        const T b3 = b[i + 3]+a[i + 3];
//        const T b4 = b[i + 4]+a[i + 4];
//        const T b5 = b[i + 5]+a[i + 5];
//        const T b6 = b[i + 6]+a[i + 6];
//        const T b7 = b[i + 7]+a[i + 7];
//        a[i + 0] = b0;
//        a[i + 1] = b1;
//        a[i + 2] = b2;
//        a[i + 3] = b3;
//        a[i + 4] = b4;
//        a[i + 5] = b5;
//        a[i + 6] = b6;
//        a[i + 7] = b7;
//      }
//    }
//};



template<class T, int S>
void bench_sum(std::vector<T> &a,
               std::vector<T> &b,
               std::vector<T> &c) {
  const size_t n = a.size();
  const size_t ntrials = 200;

  using clock=std::chrono::high_resolution_clock;

  std::chrono::time_point<clock> start_time, end_time;

  double min_time_ns=std::numeric_limits<double>::max();

  for (size_t trial = 0; trial < ntrials; trial++) {

    start_time = clock::now();
    MySum<T, S>::apply(a, b, c, n);
    end_time = clock::now();

    const double nanosec=std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    if (min_time_ns>nanosec)
    {
      min_time_ns = nanosec;
    }
  }
  const double gflops = double(n ) / (min_time_ns);
  for (size_t i = 0; i < 16; i++)
    std::cout << a[i] << " ";
//  std::cout << "a[" <<i<<"]="<<a[i]<<",";
  std::cout << std::endl;
  std::cout << "GFlops=" << gflops << std::endl;
  return;

}

//template <class T>
//void test_sum2<T,4>(std::vector<T> & a,std::vector<T> & b,std::vector<T> & c,size_t n){
//    for (size_t i = 0; i < n; i += 4) {
//      const T b0 = b[i + 0];
//      const T b1 = b[i + 1];
//      const T b2 = b[i + 2];
//      const T b3 = b[i + 3];
//      a[i + 0] += b0;
//      a[i + 1] += b1;
//      a[i + 2] += b2;
//      a[i + 3] += b3;
//    }
//  }
//}
//  end = std::chrono::system_clock::now();
//  std::chrono::duration<double> elapsed_seconds = end - start;
//  const double gflops=double(n*ntrials)/(elapsed_seconds.count()*1.e9);
//  for (size_t i=0 ; i< 16 ; i++)
//    std::cout <<a[i]<<" ";
////  std::cout << "a[" <<i<<"]="<<a[i]<<",";
//  std::cout<<std::endl;
//  std::cout << "GFlops=" << gflops << std::endl;
//  return ;
//
//}



int main( int argc,  char *argv[] )
{
  INFOS("MultiThomasTest");

  using RealType=double;
  const size_t n = 2 << 10;
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


  bench_sum<RealType,8>(a,b,c);



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




