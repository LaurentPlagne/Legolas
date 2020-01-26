#include <cstdlib>
#include <cmath>
#include <iostream>
#include <chrono>
#include "UTILITES.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

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


int main( int argc,  char *argv[] )
{
  INFOS("MultiThomasTest");
  using RealType=float;

  // Default definition for a 2D Legolas Array of floats
    using V2D=Legolas::Array<RealType,2>;

  // The following V2D type definition modifies the 2D Legolas++ Array internal layout
  // in order to get an AVX version of the ThomasSolver algorithm. The y (D=2)
  // components are grouped by pack of 8:
  // Comment the previous V2D definition and uncomment the following
//  using V2D=Legolas::Array<RealType,2,8,2>;

  const size_t nx=64;
  const size_t ny=nx*nx;

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

  //Apply the ThomasSolver algorithm to all js.
  auto start_time = std::chrono::high_resolution_clock::now();
  Legolas::map(ThomasSolver(),D2D,U2D,L2D,B2D,X2D);
  auto end_time = std::chrono::high_resolution_clock::now();
  const double microsec=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
  std::cout << "Time="<<microsec<<" us for solving ny="<<ny<<" Tridiagonal Sytems of size nx="<<nx<<std::endl;
  // Replace map with parmap to get a multi-threaded version
//  Legolas::parmap(ThomasSolver(),D2D,U2D,L2D,B2D,X2D);



  return 0;
}




