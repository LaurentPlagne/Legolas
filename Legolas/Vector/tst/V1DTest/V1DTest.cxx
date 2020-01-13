#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/ExpressionTest.hxx"

#include <list>

using Legolas::SizeType;


template <class V1D>
struct TestV1D{
  
  typedef typename V1D::Element RealType;

  static void apply( void ){
    // Default Ctor
    { 
      V1D v;
      SizeType vSize=10;
      v.resize(vSize);
      for (SizeType i=0 ; i<vSize ; i++){
	v[i]=RealType(i);
      }
      INFOS("v="<<v);
    }
    
    // Copy Ctor
    { 
      V1D v;
      SizeType vSize=10;
      v.resize(vSize);
      for (SizeType i=0 ; i<vSize ; i++) v[i]=RealType(i);
      
      V1D v2(v);
      INFOS("v2="<<v2);
      
      ASSERT(v2==v);
    }

    //Default Ctor + Copy Ctor 
    {
      V1D v;
      V1D v2(v);

      std::vector<V1D> vv(100);
    }
    

    //Test de Swap
    {
      V1D v1(100);
      V1D v2(100);

      v1=1.0;
      v2=2.0;

      Legolas::swap(v1,v2);
      
      v2*=2.0;

      double small=Legolas::norm2(v1-v2);
      
      INFOS("small="<<small);

      if (small>1.e-8) throw std::runtime_error("small>1e-8");
    }
    

    

    

    // SizeType Ctor
    { 
      SizeType vSize=10;
      V1D v(vSize);
      for (SizeType i=0 ; i<vSize ; i++) v[i]=RealType(i);
      INFOS("v="<<v);
    }
  
  
    //VECTOR EXPRESSIONS

    // SizeType Ctor
    { 
      SizeType vSize=10;
      V1D v(vSize);
      for (SizeType i=0 ; i<vSize ; i++) v[i]=RealType(i);
      Legolas::ExpressionTest::apply(v);
    }
  }
};
    


int main( int argc,  char *argv[] )
{

  try{

    TestV1D< Legolas::Vector<float> >::apply();
    TestV1D< Legolas::Vector<double> >::apply();
    
    TestV1D< Legolas::MultiVector<float,1> >::apply();
    TestV1D< Legolas::MultiVector<double,1> >::apply();

  }
  catch(const std::exception& e){
    std::cerr << "ERREUR : " << e.what() << std::endl;
  }

  
}




    
