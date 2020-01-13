#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/ExpressionTest.hxx"

#include <list>
#include <cassert>


namespace Legolas{

  template <class DERIVED>
  void fill2DVector(BaseVector<DERIVED> & vec){
    typedef typename DERIVED::Element::Element Element;
    DERIVED & v=vec.getVR();
    const SizeType vSize1=v.size();
    for (SizeType i=0 ; i<vSize1 ; i++){
      const SizeType vSize2=v[i].size();
      for (SizeType j=0 ; j<vSize2 ; j++){
	v[i][j]=Element(vSize2*i+j+1);
      }
    }
  }
    
  
  template <class REAL_TYPE>
  void fill2DMultiVector(Legolas::MultiVector<REAL_TYPE,2> & v){
    const SizeType vSize1=v.size();
    for (SizeType i=0 ; i<vSize1 ; i++){
      const SizeType vSize2=v[i].size();
      for (SizeType j=0 ; j<vSize2 ; j++){
	v[i][j]=REAL_TYPE(vSize2*i+j+1);
      }
    }
  }




  template <class V2D>
  struct TestV2D{
    
    static void apply( void ){

      INFOS("********** TESTV2D START*****************************");
      SizeType vSize1=3;
      SizeType vSize2=4;

      // Default Ctor
      //      V2D v;
      
      typename V2D::Shape shape(vSize1,vSize2);
      V2D v(shape);

      //      v.resize(vSize1);
      //      for (SizeType i=0 ; i<vSize1 ; i++){
      //	v[i].resize(vSize2);
      //      }

      v=1.0;
      INFOS("v=1.0 -> v="<<v);
      
      Legolas::fill2DVector(v);
      
      INFOS("fill2DVector(v)->v="<<v);
      
      // Copy Ctor
      V2D vcopy(v);
      ASSERT(vcopy==v);
      INFOS("vcopy(v)->vcopy="<<vcopy);
      
      assert(vcopy==v);
      
      // Size Ctor    
      V2D vs(shape);
      //      for (SizeType i=0 ; i<vSize1 ; i++){
      //	vs[i].resize(vSize2);
      //      }
      vs=v;
      INFOS("SizeCtor + vs=v ->vs="<<vs);
      
      //MultiSize Ctor :
      //      Legolas::CompressedVector< SizeType > mSizes(vSize1,vSize2);
      
      typename V2D::Shape mSizes(vSize1,vSize2);
      
      V2D vms(mSizes);

      vms=0.0;

      INFOS("vms(mSizes)-> vms="<<vms);

      vms=1.0;

      INFOS("vms=1.0-> vms="<<vms);
      
      INFOS("Legolas::dot(vms,vms)="<<Legolas::dot(vms,vms));
      assert(std::abs(Legolas::dot(vms,vms)-vSize1*vSize2)<1.e-8);


      vms+=2.0*vms;
      INFOS("vms=1.0-> vms="<<vms);
      INFOS("Legolas::dot(vms,vms)="<<Legolas::dot(vms,vms));
      INFOS("9.0*vSize1*vSize2="<<9.0*vSize1*vSize2);
      assert(std::abs(Legolas::dot(vms,vms)-9.0*vSize1*vSize2)<1.e-8);

      vms=v;
      INFOS("vms=v->vms="<<vms);


      V2D v2(v);

      v=v2+vms;
      INFOS("v=v2+vms="<<v);

      V2D v3(v);
      v3=1.0;
      INFOS("v3="<<v3);
      INFOS("v2="<<v2);
      INFOS("v="<<v);
      INFOS("vms="<<vms);
      v=v2+v3+vms;

      INFOS("v=v2+v3+vms"<<v);
      
      Legolas::ExpressionTest::apply(v);

      
      //Default Ctor + Copy Ctor 
      {
	V2D v;
	V2D v2(v);
	
	std::vector<V2D> vv(100);
      }


      INFOS("Default Ctor + Assign Operator");
      {
	V2D v;

	V2D vbis;

	vbis=v;

	INFOS("vbis="<<v);
	
	V2D v2(shape);


	v2=2.0;

	v=v2;

	INFOS("v="<<v);
	
      }
      
      
      //Test de Swap
      {
	V2D v1(shape);
	V2D v2(shape);
	
	v1=1.0;
	v2=2.0;
	
	Legolas::swap(v1,v2);
	
	v2*=2.0;
	
	double small=Legolas::norm2(v1-v2);
	
	INFOS("small="<<small);
	
	if (small>1.e-8) throw std::runtime_error("small>1e-8");
      }
      


      

      INFOS("********** TESTV2D END*****************************");

    }
  };
}


int main( int argc,  char *argv[] )
{

  typedef double RealType;



  {

    Legolas::TestV2D< Legolas::MultiVector<double,2> >::apply();
    Legolas::TestV2D< Legolas::MultiVector<float,2> >::apply();


    Legolas::MultiVector<double,2>::Shape shape2D(3,4);
    Legolas::MultiVector<double,2> X2D(shape2D);

    X2D=0.0;

    INFOS("X2D="<<X2D);
    X2D[1]=1.0;
    X2D[2]=3.0;
    
    INFOS("X2D="<<X2D);
    
    //  Legolas::MultiVector<double,2>::Element & X1Dref=X2D[1];
    Legolas::MultiVector<double,1> & X1Dref=X2D[1];
    
    INFOS("X1Dref="<<X1Dref);
    
    X1Dref*=2.0;
    INFOS("X1Dref="<<X1Dref);
    INFOS("X2D="<<X2D);
    
    fill2DMultiVector(X2D);
    INFOS("X2D="<<X2D);
    
    
    // Instead one must use  Legolas::Vector<double>
    Legolas::Vector<double> X(3);
    INFOS("X="<<X);
    // The MultiVector<double,1> Type can be used via the copy Ctor
    INFOS("X2D[1]="<<X2D[1]);
    Legolas::MultiVector<double,1> X1D(X2D[1]);
    //  INFOS("X1D="<<X1D);
    
  }

  {

    Legolas::TestV2D< Legolas::MultiVector<double,2> >::apply();
    Legolas::TestV2D< Legolas::MultiVector<float,2> >::apply();


    Legolas::MultiVector<double,2>::Shape shape2D(3,4);
    Legolas::MultiVector<double,2> X2D(shape2D);

    X2D=0.0;

    INFOS("X2D="<<X2D);
    X2D[1]=1.0;
    X2D[2]=3.0;
    
    INFOS("X2D="<<X2D);
    
    Legolas::MultiVector<double,1> & X1Dref=X2D[1];
    
    INFOS("X1Dref="<<X1Dref);
    
    X1Dref*=2.0;
    INFOS("X1Dref="<<X1Dref);
    INFOS("X2D="<<X2D);
    
    fill2DMultiVector(X2D);
    INFOS("X2D="<<X2D);

    
  }
}


    
