#include <cstdlib>
#include <cmath>
#include <iostream>
#include <chrono>
#include "UTILITES.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"
#include "helper.hxx"


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

template <class AlgoBench>
void goSpeedUpBench(void){
  std::vector<int> nthreads({1,2,3,4,5,6,7,8});

  for (auto nthread : nthreads) {
    tbb::task_scheduler_init init(nthread);
    std::vector<int> sizes = makeSizes();
    const int nbPoints = sizes.size();
    std::vector<double> perfs(nbPoints);
    for (int i = nbPoints - 1; i >= 0; i--) perfs[i] = AlgoBench::evalGflops(sizes[i]);
    std::string filename(AlgoBench::name());
    filename +="_";
    filename +=stringConvert(nthread);
    filename += ".dat";
    savePerf(sizes, perfs, filename);

  }
}




int main( int argc,  char *argv[] )
{
  INFOS("MultiThomasTest");
  using RealType=float;
  goSpeedUpBench< ThomasBench<Legolas::Array<RealType,2,8,2>, ParMap > >();


//  using ThisBench=ThomasBench<Legolas::Array<RealType,2,8,2>,SeqMap>;
//
//  const double gflops=ThisBench ::evalGflops(64);
//
//  std::cout << ThisBench::name() << " gflops="<<gflops << std::endl;


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

//  goBench< ThomasBench<Legolas::Array<RealType,2>, SeqMap > >();
//  goBench< ThomasBench<Legolas::Array<RealType,2,4,2>, SeqMap > >();
//  goBench< ThomasBench<Legolas::Array<RealType,2,8,2>, SeqMap > >();
//  goBench< ThomasBench<Legolas::Array<RealType,2>, ParMap > >();
//  goBench< ThomasBench<Legolas::Array<RealType,2,4,2>, ParMap > >();
//  goBench< ThomasBench<Legolas::Array<RealType,2,8,2>, ParMap > >();

  goBench< ThomasBench<Legolas::Array<RealType,2>, SeqMap > >();
  goBench< ThomasBench<Legolas::Array<RealType,2,4,2>, SeqMap > >();
  goBench< ThomasBench<Legolas::Array<RealType,2,8,2>, SeqMap > >();
  goBench< ThomasBench<Legolas::Array<RealType,2>, ParMap > >();
  goBench< ThomasBench<Legolas::Array<RealType,2,4,2>, ParMap > >();


  return 0;
}




