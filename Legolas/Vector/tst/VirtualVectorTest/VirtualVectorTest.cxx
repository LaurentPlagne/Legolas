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
      

      //       Legolas::BlockVector & vv=v;

//       //      INFOS("vv.getVirtualSize()="<<vv.getVirtualSize());
//       INFOS("vv.size()="<<vv.size());

//       Legolas::MultiVector<double,1> & v0=static_cast<Legolas::MultiVector<double,1> &>(vv.getElement(0));
      
//       INFOS("v0="<<v0);
      
//       Legolas::BlockVector * vclone=v.clone();

//       Legolas::MultiVector<double,2> & vc=static_cast<Legolas::MultiVector<double,2> &>(*vclone);
      
//       INFOS("vc="<<vc);

      Legolas::VirtualVector & vv=v;

      INFOS("vv.size()="<<vv.size());

      Legolas::MultiVector<double,1> & v0=static_cast<Legolas::MultiVector<double,1> &>(vv.getElement(0));
      
      INFOS("v0="<<v0);
      
      Legolas::VirtualVector * vclone=v.clone();

      Legolas::MultiVector<double,2> & vc=static_cast<Legolas::MultiVector<double,2> &>(*vclone);
      
      INFOS("vc="<<vc);
      
      
      
      //      INFOS("vv.getVirtualSize()="<<vv.getVirtualElement());
      

      
    }
  };
}

int main( int argc,  char *argv[] )
{
  
  typedef double RealType;
  Legolas::TestV2D< Legolas::MultiVector<double,2> >::apply();
  
}


    
