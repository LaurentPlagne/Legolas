#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/ExpressionTest.hxx"
#include "Legolas/Vector/VectorPool.hxx"

#include <list>

int main( int argc,  char *argv[] ){
  
  {

    Legolas::MultiVector<double,2> X("(1x2,2x3)");
    X[0]=0.0; X[1]=1.0 ; X[2]=2.0; 
    std::cout << "X=" << X << std::endl;
    
    Legolas::MultiVector<double,1> & X1D0=X[0];
    X1D0=3.0;
    std::cout << "X=" << X << std::endl;

  }
    
  //  2D Legolas++ Vector
  Legolas::MultiVector<double,2>::Shape shape;
  shape.appendElement(2);     
  shape.appendNElement(2,3);     
  
  Legolas::MultiVector<double,2> X(shape);
  
  int counter=0;
  for (int i=0 ; i<X.size() ; i++){
    Legolas::MultiVector<double,1> & Xi=X[i];
    for (int j=0 ; j<Xi.size() ; j++){
      Xi[j]=double(counter);
      counter++;
    }
  }
  std::cout << "X=" << X << std::endl;

  {

    //  2D Legolas++ Vector
    Legolas::MultiVector<double,2>::Shape shape1("(1x2,2x3)");
    Legolas::MultiVector<double,2>::Shape shape2("(2x2,1x3)");

    Legolas::VectorPool<double,2> vectorPool;

    Legolas::MultiVector<double,2> * XPtr=0;

    {
    
      Legolas::MultiVector<double,2> & X=*vectorPool.aquireVectorPtr(shape1);
      XPtr=&X;
      
      X=1.0;
      std::cout << "X=" << X << std::endl;
      vectorPool.releaseVectorPtr(&X);
    }

    {

      Legolas::MultiVector<double,2> & X=*vectorPool.aquireVectorPtr(shape1);
      //Should be the same pointer
      INFOS("&X="<<(&X));
      assert(XPtr==&X);
      X=2.0;
      std::cout << "X=" << X << std::endl;
      
      vectorPool.releaseVectorPtr(&X);
    }
    
    {
      Legolas::MultiVector<double,2> Y(shape2);
      Y=5.0;
      INFOS("Y="<<Y);


      //Should be another pointer

      Legolas::MultiVector<double,2> & X=*vectorPool.aquireVectorPtr(shape2);
      XPtr=&X;
      INFOS("&X="<<(&X));
      X=3.0;
      std::cout << "X=" << X << std::endl;
      
      vectorPool.releaseVectorPtr(&X);
    }

    {
      //Should be the same pointer
      Legolas::MultiVector<double,2> & X=*vectorPool.aquireVectorPtr(shape2);
      INFOS("&X="<<(&X));
      assert(XPtr==&X);
      X=4.0;
      std::cout << "X=" << X << std::endl;
      
      vectorPool.releaseVectorPtr(&X);
    }

    {
      //Should be the same pointer
      //      Legolas::VectorPool<double,1> vectorPool1D;

      //      Legolas::MultiVector<double,1> & X=*vectorPool1D.aquireVectorPtr(10);
      Legolas::MultiVector<double,1> & X=*vectorPool.aquireVectorPtr(10);
      INFOS("&X="<<(&X));
      X=4.0;
      std::cout << "X=" << X << std::endl;
      
      vectorPool.releaseVectorPtr(&X);
    }

    

  }

  return 0;
}


