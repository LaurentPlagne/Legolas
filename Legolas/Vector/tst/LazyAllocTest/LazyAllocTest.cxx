#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/ExpressionTest.hxx"

#include <list>

using Legolas::SizeType;

template <class VECTOR_1D>
void crashTest(const VECTOR_1D & v){
  VECTOR_1D tempo(v.size());
  for (SizeType i=0 ; i<tempo.size() ; i++){
    tempo[i]=v[i];
  }
}

template <class VECTOR_1D>
void crashTest2(const VECTOR_1D & v){
  VECTOR_1D tempo(v);
  for (SizeType i=0 ; i<tempo.size() ; i++){
    tempo[i]=v[i];
  }
}
    



int main( int argc,  char *argv[] )
{

  typedef Legolas::MultiVector<double,2> V2D;
  
  //MultiSize Ctor :

  SizeType vSize1=2;
  SizeType vSize2=3;

  //  Legolas::CompressedVector< SizeType > mSizes(vSize1,vSize2);

  V2D::Shape mSizes(vSize1,vSize2);
  
  //  Legolas::Vector< SizeType > mSizes(vSize1,vSize2);  

  V2D vms(mSizes);

  vms=0.0;

  INFOS("vms=0.0 -> vms="<<vms);
  INFOS("Legolas::dot(vms,vms)="<<Legolas::dot(vms,vms));
  assert(Legolas::dot(vms,vms)<1.e-8);
  

  vms*=2.0;
  

  INFOS("vms*=2.0 -> vms="<<vms);
  INFOS("Legolas::dot(vms,vms)="<<Legolas::dot(vms,vms));
  assert(Legolas::dot(vms,vms)<1.e-8);

  vms[0]=1.0;
  
  INFOS("vms[0]=1.0 -> vms="<<vms);
  INFOS("Legolas::dot(vms,vms)="<<Legolas::dot(vms,vms));
  assert(Legolas::dot(vms,vms)-vSize2<1.e-8);

  
  vms*=2.0;
  
  INFOS("vms*=2.0 -> vms="<<vms);
  INFOS("Legolas::dot(vms,vms)="<<Legolas::dot(vms,vms));
  assert(Legolas::dot(vms,vms)-2.0*vSize2*vSize2<1.e-8);

  V2D Y(vms);

  INFOS("Legolas::dot(X+vms,X+vms)="<<Legolas::dot(Y+vms,Y+vms));


  vms=1.0;
  
  INFOS("vms=1.0 -> vms="<<vms);
  INFOS("Legolas::dot(vms,vms)="<<Legolas::dot(vms,vms));
  assert(Legolas::dot(vms,vms)-vSize1*vSize2<1.e-8);

  //  V2D::Element X(vSize2);
  V2D::Element & X(vms[0]);
  INFOS("X"<<X);
  X[0]=4.0;
  INFOS("X"<<X);
  INFOS("vms"<<vms);

  V2D v1(mSizes);

  v1[0]=1.0;
  INFOS("v1="<<v1);

  INFOS("Legolas::dot(v1+v1,v1-v1)="<<Legolas::dot(v1+v1,v1-v1));

  V2D v2(v1+vms);

  INFOS("v2(v1+vms)="<<v2);

  crashTest(v2[1]);

  INFOS("v1="<<v1);

  const V2D & v1cr=v1;

  INFOS("v1="<<v1);
  INFOS("v1cr="<<v1cr);

  INFOS("v1cr[1]="<<v1cr[1]);

  INFOS("v1cr="<<v1cr);

  //  const V2D & v1cr=v1;

  crashTest2(v1cr[1]);

  INFOS("v1[1]="<<v1[1]);
  
  
  
  Legolas::MultiVector<double,2>::Shape shape2D;

  shape2D.appendElement(2);
  shape2D.appendElement(3);

  Legolas::MultiVector<double,3>::Shape shape3D;
  shape3D.appendNElement(4,shape2D);
  INFOS("shape3D="<<shape3D);

  Legolas::MultiVector<double,3> X3D(shape3D);

  X3D=1.0;

  INFOS("X3D="<<X3D);

  Legolas::MultiVector<double,3> Y3D(X3D);

  INFOS("Y3D="<<Y3D);


  Legolas::MultiVector<double,3> Z3D;
  Z3D=X3D;

  INFOS("Z3D="<<Z3D);

  Z3D=X3D+Y3D;

  INFOS("Z3D="<<Z3D);



  


  


}




    
