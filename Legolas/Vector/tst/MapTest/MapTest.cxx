#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/ExpressionTest.hxx"

#include <list>

using Legolas::SizeType;

struct myexp{
  typedef float RealType;
  inline RealType operator ()(const RealType & x) const{
    return std::exp(x);
  }
};

struct myrand{
  typedef double RealType;
  inline RealType operator ()(const RealType & x) const{
    return std::rand()/double(RAND_MAX);
  }
};



template <class V1D>
struct TestV1D{
  
  typedef typename V1D::Element RealType;

  static void apply( void ){
 

  
    //MAP VECTOR EXPRESSIONS

    // SizeType Ctor
    { 

      const SizeType vSize=100000000;

      V1D v1(vSize);
      V1D v2(vSize);

      //      v2=1.0;

      for (SizeType i=0 ; i< vSize ; i++) v1[i]=std::rand()/double(RAND_MAX);

      //      v1=Legolas::map(myrand(),v2);
      for (int i=0 ; i<10 ; i++) INFOS("v1["<<i<<"]="<<v1[i]);


      v1=Legolas::map(myexp(),v1);
      //      v2=Legolas::map(exp,v1);
      v1=Legolas::map(log,v1);
	
      
      for (int i=0 ; i<10 ; i++) INFOS("v2["<<i<<"]="<<v2[i]);
      for (int i=0 ; i<10 ; i++) INFOS("v1["<<i<<"]="<<v1[i]);


      v2=v1*Legolas::map(log,v1);

      //      INFOS("v2="<<v2);
      //      INFOS("v1="<<v1);
      
    }
  }
};
    


int main( int argc,  char *argv[] )
{



  TestV1D< Legolas::RectangularMultiVector<float,1> >::apply();

  Legolas::MultiVector<double,2>::Shape shape2D(3,4);
  Legolas::MultiVector<double,2> X2D(shape2D);

  X2D=1.0;

  X2D=Legolas::map(exp,X2D);

  INFOS("X2D="<<X2D);

  Legolas::MultiVector<double,1> X1D(4);

  X1D=Legolas::map(log,X2D[0]);

  INFOS("X1D="<<X1D);


  
  
  

  //  TestV1D< Legolas::RectangularMultiVector<double,1> >::apply();


  
}




    
