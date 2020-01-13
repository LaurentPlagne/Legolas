# include "UTILITES.hxx"

#include <iostream>
#include "X86Timer.hxx"
#include <vector>
#include <map>
#include <string>
#include <cmath>

#include "Legolas/Vector/Vector.hxx"

using namespace std;

int main( int argc,  char *argv[] )
{
  typedef float RealType;
  RealType epsilon=1.e-7;
  X86Timer chronos;
  
  chronos.start();
  {
    Legolas::RectangularMultiVector<RealType,2>::Shape s2(10,100);
    Legolas::RectangularMultiVector<RealType,3>::Shape s3(10,s2);
    Legolas::RectangularMultiVector<RealType,4>::Shape s4(10,s3);
    Legolas::RectangularMultiVector<RealType,5>::Shape s5(10,s4);
    Legolas::RectangularMultiVector<RealType,6>::Shape s6(10,s5);
    Legolas::RectangularMultiVector<RealType,7>::Shape s7(10,s6);

    //   int size=100000000;
    Legolas::RectangularMultiVector<RealType,7> TOTO(s7);
    
    for (int i=0 ; i< 100 ; i++){
      TOTO=0.0;
    }
  }
  chronos.stop();
  INFOS("chronos.get_elapsed_time_in_second()="<<chronos.get_elapsed_time_in_second());
  
  




  int size=20000000;
  //int size=20000;
  int sizeIn=1000;

  //  int size=4;
  //  int sizeIn=2;

  int sizeOut=size/sizeIn;
  // ****************  C vectors initialization *******************
  typedef RealType * CV1D;
  typedef CV1D *     CV2D;

  CV1D X_C=new RealType[size];
  CV1D Y_C=new RealType[size];
  CV1D Z_C=new RealType[size];
  
  for (int i = 0 ; i < size ; i++){
    X_C[i]=drand48();
    Y_C[i]=drand48();
    Z_C[i]=drand48();
  }

  CV2D X_C2D=new CV1D[sizeOut];
  CV2D Y_C2D=new CV1D[sizeOut];
  CV2D Z_C2D=new CV1D[sizeOut];

  for (int i = 0 ; i < sizeOut ; i++){
    X_C2D[i]=new RealType[sizeIn];
    Y_C2D[i]=new RealType[sizeIn];
    Z_C2D[i]=new RealType[sizeIn];
    for (int j = 0 ; j < sizeIn ; j++){
      X_C2D[i][j]=X_C[i+sizeOut*j];
      Y_C2D[i][j]=Y_C[i+sizeOut*j];
      Z_C2D[i][j]=Y_C[i+sizeOut*j];
    }
  }


  // ****************  STL vectors initialization *******************

  typedef vector< vector< RealType > > STLV2D;
  typedef vector< RealType > STLV1D;

  STLV1D X_STL,Y_STL,Z_STL;
  STLV2D X_STL2D,Y_STL2D,Z_STL2D;

  X_STL.resize(size);
  Y_STL.resize(size);
  Z_STL.resize(size);
  for (int i = 0 ; i < size ; i++){
    X_STL[i]=X_C[i];
    Y_STL[i]=Y_C[i];
    Z_STL[i]=Z_C[i];
  }


  X_STL2D.resize(sizeOut);
  Y_STL2D.resize(sizeOut);
  Z_STL2D.resize(sizeOut);

  for (int i = 0 ; i < sizeOut ; i++){
    X_STL2D[i].resize(sizeIn);
    Y_STL2D[i].resize(sizeIn);
    Z_STL2D[i].resize(sizeIn);
    for (int j = 0 ; j < sizeIn ; j++){
      X_STL2D[i][j]=X_STL[i+sizeOut*j];
      Y_STL2D[i][j]=Y_STL[i+sizeOut*j];
      Z_STL2D[i][j]=Y_STL[i+sizeOut*j];
    }
  }

  // **************** LEGOLAS  vectors initialization *******************
  
  typedef Legolas::SizeType SizeType;

  //  typedef Legolas::VectorSkin<RealType> LGS_V1D;
  //  typedef Legolas::Vector1D<RealType> LGS_V1D;
  //  typedef Legolas::VectorSkin<LGS_V1D>  LGS_V2D;

  //  typedef Legolas::Vector<RealType>        LGS_V1D;
  //  typedef Legolas::MultiVector<RealType,2> LGS_V2D;
  typedef Legolas::RectangularMultiVector<RealType,1> LGS_V1D;
  typedef Legolas::RectangularMultiVector<RealType,2> LGS_V2D;

  
  LGS_V1D X_LEGOLAS(X_STL.size());
  LGS_V1D Y_LEGOLAS(Y_STL.size());
  LGS_V1D Z_LEGOLAS(Y_STL.size());

  for (SizeType i=0 ; i < X_LEGOLAS.size() ; i++) X_LEGOLAS[i]=X_STL[i];
  for (SizeType i=0 ; i < Y_LEGOLAS.size() ; i++) Y_LEGOLAS[i]=Y_STL[i];
  for (SizeType i=0 ; i < Z_LEGOLAS.size() ; i++) Z_LEGOLAS[i]=Z_STL[i];

  LGS_V2D::Shape shape(X_STL2D.size(),X_STL2D[0].size());

  //  LGS_V2D X_LEGOLAS2D(X_STL2D.size());
  //  LGS_V2D Y_LEGOLAS2D(Y_STL2D.size());

  LGS_V2D X_LEGOLAS2D(shape);
  LGS_V2D Y_LEGOLAS2D(shape);
  LGS_V2D Z_LEGOLAS2D(shape);

  for (SizeType i=0 ; i < X_LEGOLAS2D.size() ; i++){
    for (SizeType j=0 ; j < X_LEGOLAS2D[i].size() ; j++){
      X_LEGOLAS2D[i][j]=X_STL2D[i][j];
    }
  }

  for (SizeType i=0 ; i < Y_LEGOLAS2D.size() ; i++){
    for (SizeType j=0 ; j < Y_LEGOLAS2D[i].size() ; j++){
      Y_LEGOLAS2D[i][j]=Y_STL2D[i][j];
    }
  }

  for (SizeType i=0 ; i < Z_LEGOLAS2D.size() ; i++){
    for (SizeType j=0 ; j < Z_LEGOLAS2D[i].size() ; j++){
      Z_LEGOLAS2D[i][j]=Z_STL2D[i][j];
    }
  }
  
  
  std::map< std::string, std::map<std::string,double> > times;
  
  //***********     X1D+=Y1D  *****************
      double eTime=0.0;
  
  chronos.start();
  
  for (int i=0 ; i<size ; i++){
    X_C[i]+=Y_C[i];
  }
  
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X+=Y "]["C"]=eTime;

  
  chronos.start();
  for (int i=0 ; i<size ; i++){
    X_STL[i]+=Y_STL[i];
  }
  chronos.stop();
  
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X+=Y "]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS+=Y_LEGOLAS;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X+=Y "]["LEGOLAS"]=eTime;
  
  //***********     X2D+=Y2D  *****************

      chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_C2D[i][j]+=Y_C2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X+=Y "]["C"]=eTime;

  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_STL2D[i][j]+=Y_STL2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X+=Y "]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS2D+=Y_LEGOLAS2D;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X+=Y "]["LEGOLAS"]=eTime;

  //***************** scale 1D **************

      
      RealType factor=2.0;
  
  chronos.start();
  for (int i=0 ; i<size ; i++){
    X_C[i]*=factor;
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D Scale"]["C"]=eTime;

  chronos.start();
  for (int i=0 ; i<size ; i++){
    X_STL[i]*=factor;
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D Scale"]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS*=factor;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D Scale"]["LEGOLAS"]=eTime;

  //***************** scale 2D **************

      
      chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_C2D[i][j]*=factor;
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D Scale"]["C"]=eTime;

  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_STL2D[i][j]*=factor;
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D Scale"]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS2D*=factor;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D Scale"]["LEGOLAS"]=eTime;


  //***************** dot<X1D,Y1D> **************

      {
	double scalarProductC=0.0;
	chronos.start();
	for (int i=0 ; i<size ; i++){
	  scalarProductC+=X_C[i]*Y_C[i];
	}
	chronos.stop();
	eTime=chronos.get_elapsed_time_in_second();
	times["1D DOT  "]["C"]=eTime;
	
	
	double scalarProductSTL=0.0;
	chronos.start();
	for (int i=0 ; i<size ; i++){
	  scalarProductSTL+=X_STL[i]*Y_STL[i];
	}
	chronos.stop();
	eTime=chronos.get_elapsed_time_in_second();
	times["1D DOT  "]["STL"]=eTime;
	
	double scalarProductLEGOLAS=0.0;
	chronos.start();
	scalarProductLEGOLAS=Legolas::dot(X_LEGOLAS,Y_LEGOLAS);
	chronos.stop();
	
	if (std::abs(scalarProductC-scalarProductSTL)/scalarProductC>epsilon) INFOS("Erreur ds le produit scalaire 1D");
	
	double diff=(scalarProductC-scalarProductLEGOLAS)/scalarProductC;
	if (std::abs(diff)>epsilon){
	  INFOS("Erreur ds le produit scalaire 1D");
	  INFOS("scalarProductLEGOLAS="<<scalarProductLEGOLAS);
	  INFOS("scalarProductC="<<scalarProductC);
	  INFOS("diff="<<diff);
	}

	eTime=chronos.get_elapsed_time_in_second();
	times["1D DOT  "]["LEGOLAS"]=eTime;
      }
  
  
  
  //***************** dot<X2D,Y2D> **************
      {
	double scalarProductC=0.0;
	chronos.start();
	for (int i = 0 ; i < sizeOut ; i++){
	  for (int j = 0 ; j < sizeIn ; j++){
	    scalarProductC+=X_C2D[i][j]*Y_C2D[i][j];
	  }
	}
	chronos.stop();
	eTime=chronos.get_elapsed_time_in_second();
	times["2D DOT  "]["C"]=eTime;
	
	
	double scalarProductSTL=0.0;
	chronos.start();
	for (int i = 0 ; i < sizeOut ; i++){
	  for (int j = 0 ; j < sizeIn ; j++){
	    scalarProductSTL+=X_STL2D[i][j]*Y_STL2D[i][j];
	  }
	}
	chronos.stop();
	eTime=chronos.get_elapsed_time_in_second();
	times["2D DOT  "]["STL"]=eTime;
	

// 	for (int i = 0 ; i < sizeOut ; i++){
// 	  for (int j = 0 ; j < sizeIn ; j++){
// 	    INFOS("i="<<i<<" , "<<j<<" XSTL="<<X_STL2D[i][j]<<" YSTL="<<Y_STL2D[i][j]<<" XLGS="<<X_LEGOLAS2D[i][j]<<" YLGS="<<Y_LEGOLAS2D[i][j]);
// 	  }
// 	}



	double scalarProductLEGOLAS=0.0;
	chronos.start();
	scalarProductLEGOLAS=Legolas::dot(X_LEGOLAS2D,Y_LEGOLAS2D);
	chronos.stop();
	
	if (abs(scalarProductC-scalarProductSTL)/scalarProductC>epsilon) INFOS("Erreur ds le produit scalaire 2D");

	double diff=(scalarProductC-scalarProductLEGOLAS)/scalarProductC;
	if (abs(diff)>epsilon){
	  INFOS("Erreur ds le produit scalaire 2D");
	  INFOS("scalarProductLEGOLAS="<<scalarProductLEGOLAS);
	  INFOS("scalarProductC="<<scalarProductC);
	  INFOS("diff="<<diff);
	}
      }

  eTime=chronos.get_elapsed_time_in_second();
  times["2D DOT  "]["LEGOLAS"]=eTime;

  //*****************  1D X=Y  **************
      
      chronos.start();
  for (int i=0 ; i<size ; i++){
    X_C[i]=Y_C[i];
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X=Y  "]["C"]=eTime;


  chronos.start();
  for (int i=0 ; i<size ; i++){
    X_STL[i]=Y_STL[i];
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X=Y  "]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS=Y_LEGOLAS;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X=Y  "]["LEGOLAS"]=eTime;

  //***************** 2D X=Y **************

      
      chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_C2D[i][j]=Y_C2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X=Y  "]["C"]=eTime;

  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_STL2D[i][j]=Y_STL2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X=Y  "]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS2D=Y_LEGOLAS2D;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X=Y  "]["LEGOLAS"]=eTime;

  
  /***********     Z=2*X-3*Y 1D  *****************/
      
  chronos.start();
  for (int i=0 ; i<size ; i++){
    Z_C[i]=2.0*X_C[i]+3.0*Y_C[i];
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D Z=2*X-3*Y"]["C"]=eTime;
  

  chronos.start();
  for (int i=0 ; i<size ; i++){
    Z_STL[i]=2.0*X_STL[i]+3.0*Y_STL[i];
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D Z=2*X-3*Y"]["STL"]=eTime;

  chronos.start();
  Z_LEGOLAS=2.0*X_LEGOLAS+3.0*Y_LEGOLAS;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D Z=2*X-3*Y"]["LEGOLAS"]=eTime;


  /***********     Z=2*X-3*Y 2D  *****************/

  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      Z_C2D[i][j]=2.0*X_C2D[i][j]+3.0*Y_C2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D Z=2*X-3*Y"]["C"]=eTime;

  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      Z_STL2D[i][j]=2.0*X_STL2D[i][j]+3.0*Y_STL2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D Z=2*X-3*Y"]["STL"]=eTime;

  chronos.start();
  Z_LEGOLAS2D=2.0*X_LEGOLAS2D+3.0*Y_LEGOLAS2D;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D Z=2*X-3*Y"]["LEGOLAS"]=eTime;

  /***********     axpy 1D  *****************/
      
  chronos.start();
  factor=3.0;
  for (int i=0 ; i<size ; i++){
    X_C[i]+=factor*Y_C[i];
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D AXPY "]["C"]=eTime;
  

  chronos.start();
  for (int i=0 ; i<size ; i++){
    X_STL[i]+=factor*Y_STL[i];
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D AXPY "]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS+=factor*Y_LEGOLAS;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D AXPY "]["LEGOLAS"]=eTime;

  /***********     axpy 2D  *****************/

  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_C2D[i][j]+=factor*Y_C2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D AXPY "]["C"]=eTime;

  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_STL2D[i][j]+=factor*Y_STL2D[i][j];
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D AXPY "]["STL"]=eTime;

  chronos.start();
  X_LEGOLAS2D+=factor*Y_LEGOLAS2D;
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D AXPY "]["LEGOLAS"]=eTime;
  
  //***********     X1D=0.0  *****************

      chronos.start();
  
  for (int i=0 ; i<size ; i++){
    X_C[i]=0.0;
  }
  
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X=0  "]["C"]=eTime;

  chronos.start();
  
  for (int i=0 ; i<size ; i++){
    X_STL[i]=0.0;
  }

  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X=0  "]["STL"]=eTime;

  chronos.start();
  
  X_LEGOLAS=0.0;
  
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["1D X=0  "]["LEGOLAS"]=eTime;

  //***********     X2D=0.0  *****************

      chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_C2D[i][j]=0.0;
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X=0  "]["C"]=eTime;


  chronos.start();
  for (int i = 0 ; i < sizeOut ; i++){
    for (int j = 0 ; j < sizeIn ; j++){
      X_STL2D[i][j]=0.0;
    }
  }
  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X=0  "]["STL"]=eTime;

  chronos.start();

  //X_LEGOLAS2D.setZero();
  X_LEGOLAS2D=0.0;

  chronos.stop();
  eTime=chronos.get_elapsed_time_in_second();
  times["2D X=0  "]["LEGOLAS"]=eTime;
  
  {
    std::map< std::string , std::map<std::string,double> >::iterator iter=times.begin();

    for ( ; iter !=times.end() ; iter++){
    
      const std::string & chaine=(*iter).first;
      std::map<std::string,double> & key=(*iter).second;

      double ct=key["C"];
      double st=key["STL"];
      double gt=key["LEGOLAS"];

      //      double accel=100.0*(ct-gt)/ct; // eh eh
      double speedup=ct/gt; // eh eh

      INFOS(chaine<<" Legolas++ Speed_Up : "<<speedup<<" C :"<<ct<<" STL :"<<st<<" Legolas++ : "<<gt);

      if (speedup < 0.8){
	INFOS("Si la compilation est faite en mode optimisé... il y a un pb de perf avec l'opération :");
	INFOS(chaine<<" Legolas++ Speed-Up : "<<speedup<<" C :"<<ct<<" STL :"<<st<<" Legolas++ : "<<gt);
      }

    }
  }
  
 
  // free of C Vectors

  delete[] X_C;
  delete[] Y_C;

  for (int i = 0 ; i < sizeOut ; i++){
    delete [] X_C2D[i];
    delete [] Y_C2D[i];
  }

  delete[] X_C2D;
  delete[] Y_C2D;

 
}


  

  
