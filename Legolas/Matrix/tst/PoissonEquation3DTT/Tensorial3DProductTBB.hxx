#ifndef __TENSORIAL3DPRODUCTTBB_HXX__
#define __TENSORIAL3DPRODUCTTBB_HXX__

#include "X86Timer.hxx"
#include "Legolas/Algorithm/ATLASDenseMatrixVectorProduct.hxx"
#include "Legolas/Algorithm/ATLASDenseMatrixMatrixProduct.hxx"

#include "my_tbb_parallel_for.hxx"

// U=(M_{X}\otimes M_{Y}\otimes M_{Z})S

template <class M2D, class V3D>
struct MzMyFunctor{
  M2D Mz_;
  M2D My_;
  const V3D & S_;
  V3D & U_;
  
  MzMyFunctor(const M2D & Mz,
	      const M2D & My,
	      const V3D & S,
	      V3D & U):Mz_(Mz),My_(My),S_(S),U_(U){}

  inline void operator ()(const my_tbb::blocked_range<int> & r) const {


    const int sizeY=My_.nrows();
    const int sizeZ=Mz_.nrows();
    
    M2D Szy(Legolas::MatrixShape<1>(sizeZ,sizeY));
    M2D Wzy(Legolas::MatrixShape<1>(sizeZ,sizeY));
    M2D Wyz(Legolas::MatrixShape<1>(sizeY,sizeZ));
    M2D Vyz(Legolas::MatrixShape<1>(sizeY,sizeZ));

    for (int i=r.begin() ; i!=r.end() ; i++){
      
      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  Szy(k,j)=S_[i][j][k];
	}
      }
      
      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mz_,Szy,Wzy);

      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  Wyz(j,k)=Wzy(k,j);
	}
      }
      

      Legolas::ATLASDenseMatrixMatrixProduct::apply(My_,Wyz,Vyz);
      
      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  U_[i][j][k]=Vyz(j,k);
	}
      }
    }
  }
};

template <class M2D, class V3D>
struct MxFunctor{
  M2D Mx_;
  M2D My_;
  const V3D & U_;
  V3D & Uzxy_;
  
  MxFunctor(const M2D & Mx,
	    const M2D & My,
	    const V3D & U,
	    V3D & Uzxy):Mx_(Mx),My_(My),U_(U),Uzxy_(Uzxy){}

  inline void operator ()(const my_tbb::blocked_range<int> & r) const {

    const int sizeX=Mx_.nrows();
    const int sizeY=My_.nrows();

    M2D Vxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    M2D Uxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    
    for (int k=r.begin() ; k!=r.end() ; k++){  ////Permutation en lecture!! 
      
      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){
	  Vxy(i,j)=U_[i][j][k];
	}
      }

      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mx_,Vxy,Uxy);
      

      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){

	  Uzxy_[k][i][j]=Uxy(i,j);
	}
      }
    }
  }
};
	      
	      


struct Tensorial3DProductTBB{
  


  template <class DENSE_MATRIX_2D, class VECTOR_3D>
  static void applyXYZ_ZXY(const DENSE_MATRIX_2D & Mx,
			   const DENSE_MATRIX_2D & My,
			   const DENSE_MATRIX_2D & Mz,
			   const VECTOR_3D & S,
			   VECTOR_3D & U,
			   VECTOR_3D & Uzxy){
 
    const int sizeX=Mx.nrows();
    const int sizeZ=Mz.nrows();
    
    MzMyFunctor<DENSE_MATRIX_2D,VECTOR_3D> mzmyFuntor(Mz,My,S,U);
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,sizeX),mzmyFuntor);

    MxFunctor<DENSE_MATRIX_2D,VECTOR_3D> mxFuntor(Mx,My,U,Uzxy);
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,sizeZ),mxFuntor);

  }
  

  template <class DENSE_MATRIX_2D, class VECTOR_3D>
  static void applyZXY_XYZ(const DENSE_MATRIX_2D & Mx,
			   const DENSE_MATRIX_2D & My,
			   const DENSE_MATRIX_2D & Mz,
			   VECTOR_3D & Szxy,
			   VECTOR_3D & Uxyz){
    
    X86Timer chronos;
    X86Timer chronos_tot;

    double gemmTime=0.0;
    double permTime=0.0;

    chronos_tot.start();
    chronos.start();

    // Mx Transposition
    const int sizeX=Mx.nrows();
    const int sizeY=My.nrows();
    const int sizeZ=Mz.nrows();
    
    DENSE_MATRIX_2D Sxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    DENSE_MATRIX_2D Wxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    DENSE_MATRIX_2D Wyx(Legolas::MatrixShape<1>(sizeY,sizeX));
    DENSE_MATRIX_2D Vyx(Legolas::MatrixShape<1>(sizeY,sizeX));
    DENSE_MATRIX_2D Vzy(Legolas::MatrixShape<1>(sizeZ,sizeY));
    DENSE_MATRIX_2D Uzy(Legolas::MatrixShape<1>(sizeZ,sizeY));

    for (int k=0 ; k< sizeZ ; k++){ 
      
      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){
	  Sxy(i,j)=Szxy[k][i][j];
	}
      }
      
      chronos.start();
      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mx,Sxy,Wxy);
      chronos.stop();
      gemmTime+=chronos.get_elapsed_time_in_second();

      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){
	  Wyx(j,i)=Wxy(i,j);
	}
      }
      
      chronos.start();
      Legolas::ATLASDenseMatrixMatrixProduct::apply(My,Wyx,Vyx);
      chronos.stop();
      gemmTime+=chronos.get_elapsed_time_in_second();

      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){
	  Szxy[k][i][j]=Vyx(j,i);
	}
      }
    }


    for (int i=0 ; i< sizeX ; i++){
      
      chronos.start();
      for (int k=0 ; k< sizeZ ; k++){//lecture permutée
	for (int j=0 ; j< sizeY ; j++){
	  Vzy(k,j)=Szxy[k][i][j];
	}
      }
      chronos.stop();
      permTime+=chronos.get_elapsed_time_in_second();
      
      chronos.start();
      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mz,Vzy,Uzy);
      chronos.stop();
      gemmTime+=chronos.get_elapsed_time_in_second();

      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  Uxyz[i][j][k]=Uzy(k,j);
	}
      }
    }

    chronos_tot.stop();
    double totTime=chronos_tot.get_elapsed_time_in_second();
    INFOS("totTime="<<totTime);
    INFOS("gemmTime="<<gemmTime<<" ("<<100.0*gemmTime/totTime<<"%)");
    INFOS("permTime="<<permTime<<" ("<<100.0*permTime/totTime<<"%)");

  }

};

#endif  
			
