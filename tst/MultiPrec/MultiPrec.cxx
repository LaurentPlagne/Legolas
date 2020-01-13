/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThomasSolver.cxx
 * Author: Laurent Plagne
 *
 * Created on 13 mai 2017, 13:17
 */

#include <cstdlib>
#include "UTILITES.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"
#include "thomassolver.hxx"
#include <iostream>
#include <fstream>
#include "Legolas/Array/Block/Block.hxx"

template <class V1D>
void dumpVector(const V1D & v, std::string s){
  std::ofstream myfile;
  myfile.open(s.c_str());
  for (size_t i= 0 ; i<v.size() ; i++){
    myfile << i<< " " << v[i] << std::endl;
  }
  myfile.close();
}


template <class V1L, class V1R>
void convert(const V1L & v1l, V1R & v1r){
  assert(v1l.size()==v1r.size());
  using RealType=typename V1R::RealType;
  for (size_t i=0 ; i<v1r.size() ; i++){
    v1r[i]=RealType(v1l[i]);
  }
}
//Y+=a*(LDU)*X
template <class V1D>
void LDUMultiply(const V1D & L,const V1D & D,const V1D & U,
                 const V1D & X, V1D & Y){
  const size_t nx=L.size();
  assert((nx == L.size()) && (nx==U.size()) && (nx ==Y.size()) && (nx==X.size()));
  using RealType=typename V1D::RealType;
  for (size_t i=0 ; i< nx ; i++){
    RealType s=D[i]*X[i];
    if (i>0) s+=L[i]*X[i-1];
    if (i<nx-1) s+=U[i]*X[i+1];
    Y[i]=s;
  }
}


//Y+=a*(LDU)*X
template <class V1D>
void LDUAddMultiply(typename V1D::RealType coeff,
                    const V1D & L,const V1D & D,const V1D & U,
                    const V1D & X, V1D & Y){
  const size_t nx=L.size();
  assert((nx == L.size()) && (nx==U.size()) && (nx ==Y.size()) && (nx==X.size()));
  using RealType=typename V1D::RealType;
  for (size_t i=0 ; i< nx ; i++){
    RealType s=D[i]*X[i];
    if (i>0) s+=L[i]*X[i-1];
    if (i<nx-1) s+=U[i]*X[i+1];
    Y[i]+=coeff*s;
  }
}


// Gauss Seidel Method...
template <class V1F, class V1D>
std::vector<double> convergence_gauss_seidel(int niter,
                                             const V1F & L,const V1F & D,const V1F & U,
                                             const V1F & B, const V1D & Xref, V1F & X){
  std::vector<double> residuals;
  //  std::ofstream myfile;
  //  myfile.open("residual_float.dat");

  using RealType=typename V1F::RealType;

  const size_t nx=L.size();
  assert((nx == L.size()) && (nx==U.size()) && (nx ==B.size()) && (nx==X.size()));

  V1D Xd(nx);
  //  auto vecNorm=Legolas::squaredNorm(Xref);

  for (int iter=0 ; iter<niter ; iter++){
    RealType s;
    for (size_t i=0 ; i< nx ; i++){
      s=B[i];
      if (i>0) s-=L[i]*X[i-1];
      if (i<nx-1) s-=U[i]*X[i+1];
      X[i]=s/D[i];
    }

    convert(X,Xd);
    auto residual=sqrt(Legolas::squaredNorm(X-Xref)/Legolas::squaredNorm(Xref));
    residuals.push_back(residual);

    //    myfile << iter<< " " << std::sqrt(residual/vecNorm) << std::endl;
  }

  //  myfile.close();
  INFOS("X"<<X);
  return residuals;
}

// Conjugate Gradient Method...
template <class V1F, class V1D>
std::vector<double> convergence_conjugate_gradient(int niter,
                                                   const V1F & L,const V1F & D,const V1F & U,
                                                   const V1F & B, const V1D & Xref, V1F & X){
  std::vector<double> residuals;

  const size_t nx=L.size();
  assert((nx == L.size()) && (nx==U.size()) && (nx ==B.size()) && (nx==X.size()));


  V1F R(B.shape());
  V1F P(B.shape());
  V1F Q(B.shape());

  R=B.copy();
  //R=B-AX;
  LDUAddMultiply(-1.0,L,D,U,X,R);
  P=R;
  Q=R;

  double delta=Legolas::squaredNorm(R);
  V1D Xd(nx);

  for (int iter=0 ; iter<niter ; iter++){

    //Q=A*P
    LDUMultiply(L,D,U,P,Q);

    const double alpha=delta/Legolas::dot(Q,P);

    X+=alpha*P;
    R-=alpha*Q;

    const double deltaOld=delta;
    delta=Legolas::squaredNorm(R);

    const double beta=delta/deltaOld;
    P=beta*P+R;
    convert(X,Xd);
    auto residual=sqrt(Legolas::squaredNorm(X-Xref)/Legolas::squaredNorm(Xref));
    residuals.push_back(residual);
  }
  //  myfile.close();
  INFOS("X"<<X);
  return residuals;

}


using namespace std;



/*
 *
 */
int main(int argc, char** argv) {

  INFOS("MultiPrecTest");

  const int nx = 20;

  // Compute Reference result:
  using RealType=double;
  using V1D=Legolas::Array<RealType,1>;

  V1D X(nx),B(nx),D(nx),L(nx),U(nx);

  X.fill(1.0); B.fill(1.0);
  L.fill(-1.0); D.fill(2.0); U.fill(-1.0);

  ThomasSolver ts;
  ts(D,L,U,B,X);


  V1D Xref(nx);
  Xref=X;

  INFOS("Xref"<<Xref);



  //Check Convergence for single precision
  using V1F=Legolas::Array<float,1>;

  V1F Xf(nx),Bf(nx),Df(nx),Lf(nx),Uf(nx);
  convert(X,Xf);convert(B,Bf);
  convert(L,Lf);convert(D,Df);convert(U,Uf);
  Xf.fill(1.0f);

  INFOS("Start Gauss-Seidel");
  // Gauss Seidel Method...
  auto convergence=convergence_gauss_seidel(1000,Lf,Df,Uf,Bf,Xref,Xf);
  //  auto convergence=convergence_conjugate_gradient(20,Lf,Df,Uf,Bf,Xref,Xf);

  INFOS("Start dump");
  dumpVector(convergence,"residual.dat");

  INFOS("end");

  V1D Xinf(nx);V1D AXinf(nx);V1D Binf(nx);
  convert(Xf,Xinf);
  LDUMultiply(L,D,U,Xinf,AXinf);
  INFOS("AXinf="<<AXinf);
  Binf=B;
  Binf-=AXinf;
  INFOS("Binf="<<Binf);
  V1F newBinf(nx);
  convert(Binf,newBinf);
  INFOS("newBinf="<<newBinf);

  V1D newXref(nx);
  newXref=Xref;
  newXref-=Xinf;

  auto newConvergence=convergence_gauss_seidel(1000,Lf,Df,Uf,newBinf,newXref,Xf);
  //  auto newConvergence=convergence_conjugate_gradient(20,Lf,Df,Uf,newBinf,newXref,Xf);
  dumpVector(newConvergence,"newResidual.dat");

  V1D result(nx);
  convert(Xf,result);
  result+=Xinf;
  INFOS("result"<<result);



  INFOS("Legolas::BlockShape<3>::size="<<(Legolas::BlockShape<3>::size_));
  INFOS("Legolas::BlockShape<3>::flatSize="<<(Legolas::BlockShape<3>::flatSize_));
  INFOS("Legolas::BlockShape<3,2>::size="<<(Legolas::BlockShape<3,2>::size_));
  INFOS("Legolas::BlockShape<3,2>::flatSize="<<(Legolas::BlockShape<3,2>::flatSize_));

  std::vector<double> data;
  std::vector<double> a,b,c;
  constexpr int s3=4;
  constexpr int s2=3;
  constexpr int s1=2;

  using MyShape=Legolas::BlockShape<s3,s2,s1>;
  using MyBlockView=Legolas::BlockView<double,MyShape>;
  //using MyBlock=Legolas::BlockView<double,s3,s2,s1>;

  double v=1;
  for (int k=0 ; k<s3;k++){
    for (int j=0 ; j<s2;j++){
      for (int i=0 ; i<s1;i++){
        data.push_back(v);
        a.push_back(0);
        b.push_back(0);
        c.push_back(0);
        v+=1.0;
      }
    }
  }

  //Legolas::BlockView<double,3,4,5> b(&data[0);
  MyBlockView db(&data[0]);

  std::cout << "db="<<db<<std::endl;

  MyBlockView ab(&a[0]);
  MyBlockView bb(&b[0]);
  MyBlockView cb(&c[0]);

  ab=db;

  std::cout << "ab="<<ab<<std::endl;

  cb=1;
  std::cout << "cb="<<cb<<std::endl;

  ab+=3*db-cb*2;

  std::cout << "ab="<<ab<<std::endl;


 using MyBlock=Legolas::Block<double,MyShape>;

 MyBlock b1,b2,b3;

 b1=1;
 b2=ab;
 b3=3*b1+cb;
 std::cout << "b1="<<b1<<std::endl;
 std::cout << "b2="<<b2<<std::endl;
 std::cout << "b3="<<b3<<std::endl;


  //  Legolas::Toto<Legolas::BlockShape,s3,s2,s1>::Titi a;
  //  std::cout << Legolas::Tata<s3,s2,s1>::isNoPack << std::endl;
  //std::cout << Legolas::Tata<s1>::isNoPack << std::endl;

  //  //  X1D.display();
  //  display1D(X1D);



  return 0;
}

