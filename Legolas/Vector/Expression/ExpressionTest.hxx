#ifndef __LEGOLAS_EXPRESSIONTEST_HXX__
#define __LEGOLAS_EXPRESSIONTEST_HXX__

#include <assert.h>

namespace Legolas{

  struct ExpressionTest{

    template <class VECTOR>
    static void apply(const VECTOR & v){

      INFOS("********** EXPRESSION TEST START*****************************");
      VECTOR v1(v);
      VECTOR result(v);

      //      result="a";
      result=v;
      INFOS("v="<<v);
      INFOS("result="<<result);

      result+=v;
      INFOS("result="<<result);
      INFOS("v="<<v);

      //      result+=v;
      
 //      INFOS("v1="<<v1);      

      VECTOR v2(2.0*v);

      INFOS("v2="<<v2);      

      VECTOR v3(3.0*v);
      
      INFOS("v2="<<v2);
      INFOS("v3="<<v3);
      
      result=v1+v2+v3;
      INFOS("result=v1+v2+v3="<<result);
      
      result=1.0*v1+v2*2.0+2.0*(v1+v3);
      INFOS("result=1.0*v1+v2*2.0+2.0*(v1+v3)="<<result);
      result+=1.0*v1+v2*2.0+2.0*(v1+v3);
      INFOS("result+=1.0*v1+v2*2.0+2.0*(v1+v3)="<<result);


      INFOS("Legolas::dot(result,result)="<<Legolas::dot(result,result));
      assert(Legolas::dot(result,result)>1.e-8);

      result-=2.0*(1.0*v1+v2*2.0+2.0*(v1+v3));

      INFOS("result-=2.0*(1.0*v1+v2*2.0+2.0*(v1+v3))="<<result);
      
      INFOS("Legolas::dot(result,result)="<<Legolas::dot(result,result));
      assert(Legolas::dot(result,result)<1.e-8);


      result+=(1.0*v1+v2*2.0)+(1.0*v1+v2*2.0);
      result-=2.0*(1.0*v1+v2*2.0);
      


      result+=2.0*v1;
      INFOS("result+=2.0*v1="<<result);

      assert(Legolas::dot(result,result)>1.e-8);
      result-=2.0*v1;
      INFOS("result-=2.0*v1="<<result);

      //      assert(Legolas::dot(result,result)<1.e-8);
      assert(Legolas::squareNorm2(result)<1.e-8);
      INFOS("Legolas::dot(result,result)="<<Legolas::dot(result,result));
      INFOS("Legolas::squareNorm2(result)="<<Legolas::squareNorm2(result));
      INFOS("Legolas::norm2(result)="<<Legolas::norm2(result));

      INFOS("Legolas::squareNorm2(2.0*v1-2.0*v1)="<<Legolas::squareNorm2(2.0*v1-2.0*v1));
      
      
      INFOS("Legolas::dot(v1+v2,v3)="<<Legolas::dot(v1+v2,v3));


      std::cout << "********** EXPRESSION TEST END*****************************" <<std::endl;

    }
  };

}

#endif

      
