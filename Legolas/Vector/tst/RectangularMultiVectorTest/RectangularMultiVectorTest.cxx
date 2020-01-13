#include "UTILITES.hxx"
#include "Legolas/Vector/RectangularMultiVector.hxx"
//#include "Legolas/Vector/RectangularViewMultiVector.hxx"

#include "X86Timer.hxx"
//#include "Legolas/Vector/Expression/ExpressionTest.hxx"

//#include <list>


int main( int argc,  char *argv[] )
{
  Legolas::RectangularMultiVector<double,2>::Shape shape("(3x2)");

  INFOS("shape="<<shape);

  Legolas::RectangularMultiVector<double,2> X(shape);

  INFOS("X.size()="<<X.size());
  
  for (int i=0 ; i<X.size() ; i++){
    for (int j=0 ; j<X[i].size() ; j++){
      X[i][j]=double(i+j);
    }
  }
  
  INFOS("X="<<X);


  INFOS("TEST COPY CTOR");

  Legolas::RectangularMultiVector<double,2> Y(X);

  INFOS("Y="<<Y);


  INFOS("TEST Y+=X");

  Y+=X;

  INFOS("Y="<<Y);


  INFOS("TEST Y+=X");

  Y+=X;

  INFOS("Y="<<Y);
 

  INFOS("TEST Y-=2.0*X");

  Y-=2.0*X;

  INFOS("Legolas::dot(Y,X)="<<Legolas::dot(Y,X));


  INFOS("Y="<<Y);

  Y=0.0;

  INFOS("Y="<<Y);

  
  //  chronos.start();
  {
    //    Legolas::RectangularMultiVector<double,4>::Shape shape("(2x(10000x(1x100)))");
    Legolas::RectangularMultiVector<double,4>::Shape shape("(2x(3x(4x3)))");
    Legolas::RectangularMultiVector<double,4> B(shape);
    
    int index=10;

    for (int i=0 ; i < B.size() ; i++){
      for (int j=0 ; j < B[i].size() ; j++){
	for (int k=0 ; k < B[i][j].size() ; k++){
	  for (int l=0 ; l < B[i][j][k].size() ; l++){
	    B[i][j][k][l]=double(index);
	    index++;
	  }
	}
      }
    }
	    

    INFOS("B="<<B);

    Legolas::RectangularMultiVector<double,4>::Shape newShape("(2x(4x(3x3)))");

    Legolas::RectangularMultiVector<double,4> Bmodif(newShape,B.realDataPtr());
    INFOS("Bmodif="<<Bmodif);
    
 //    {
//       //      int newShape[4]={2,3,3,3};
//       Legolas::RectangularViewMultiVector<double,4> BView(newShape,B.realDataPtr());
//       INFOS("BView="<<BView);
//     }
      

  }
  //  chronos.stop();
  //  INFOS("alloc duration #1 :"<<chronos.get_elapsed_time_in_second());
  


  X86Timer chronos;

  chronos.start();
  {
    Legolas::RectangularMultiVector<double,4>::Shape shape("(2x(10000x(1x100)))");
    Legolas::RectangularMultiVector<double,4> B(shape);
    for (int i=0 ; i<100 ; i++)
      B=1.0;
  }
  chronos.stop();
  INFOS("alloc duration #1 :"<<chronos.get_elapsed_time_in_second());
  
  chronos.start();
  {
    Legolas::RectangularMultiVector<double,4>::Shape shape("(2x(10000x(1x100)))");
    //    const int size=Legolas::getFlatSize(shape);
    const size_t size=shape.flatSize();
    double * B=new double[size];
    for (int i=0 ; i<100 ; i++)
      for (int j=0 ; j<int(size) ; j++) B[j]=1.0;
    
    delete [] B;
  }
  chronos.stop();    

  INFOS("alloc duration #2 :"<<chronos.get_elapsed_time_in_second());

  chronos.start();
  {
    Legolas::RectangularMultiVector<double,4>::Shape shape("(2x(10000x(1x100)))");
    Legolas::RectangularMultiVector<double,4> B(shape);
    B=1.0;
    
    Legolas::RectangularMultiVector<double,4>::Shape newShape("(2x(10000x(1x100)))");
    for (int i=0 ; i<100 ; i++){
      Legolas::RectangularMultiVector<double,4> Bmodif(newShape,B.realDataPtr());
      B=2.0;
    }
      
  }
  chronos.stop();    

  INFOS("alloc duration #3 :"<<chronos.get_elapsed_time_in_second());

  chronos.start();
  {
    Legolas::RectangularMultiVector<double,4>::Shape shape("(2x(10000x(1x100)))");
    Legolas::RectangularMultiVector<double,4> B(shape);
    B=1.0;
    
    Legolas::RectangularMultiVector<double,4>::Shape newShape("(2x(10000x(1x100)))");
  //   for (int i=0 ; i<100 ; i++){
//       Legolas::RectangularViewMultiVector<double,4> Bmodif(newShape,B.realDataPtr());
//       B=2.0;
//     }
      
  }
  chronos.stop();    

  INFOS("alloc duration #3 :"<<chronos.get_elapsed_time_in_second());



  
  
}



    
			   
