#ifndef __TENSORIAL3DPRODUCT_HXX__
#define __TENSORIAL3DPRODUCT_HXX__

#include "X86Timer.hxx"
#include "Legolas/Algorithm/ATLASDenseMatrixVectorProduct.hxx"
#include "Legolas/Algorithm/ATLASDenseMatrixMatrixProduct.hxx"

// U=(M_{X}\otimes M_{Y}\otimes M_{Z})S


struct Tensorial3DProductV0{
  
  template <class DENSE_MATRIX_2D, class VECTOR_3D>
  static void apply(const DENSE_MATRIX_2D & Mx,
		    const DENSE_MATRIX_2D & My,
		    const DENSE_MATRIX_2D & Mz,
		    const VECTOR_3D & S,
		    VECTOR_3D & Uzxy,
		    VECTOR_3D & U){
    
    // Mx Transposition
    const int sizeX=Mx.nrows();
    const int sizeY=My.nrows();
    const int sizeZ=Mz.nrows();

    DENSE_MATRIX_2D Szy(Legolas::MatrixShape<1>(sizeZ,sizeY));
    DENSE_MATRIX_2D Wzy(Legolas::MatrixShape<1>(sizeZ,sizeY));
    DENSE_MATRIX_2D Wyz(Legolas::MatrixShape<1>(sizeY,sizeZ));
    DENSE_MATRIX_2D Vyz(Legolas::MatrixShape<1>(sizeY,sizeZ));
    DENSE_MATRIX_2D Vxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    DENSE_MATRIX_2D Uxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    
    
    for (int i=0 ; i< sizeX ; i++){
      
      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  Szy(k,j)=S[i][j][k];
	}
      }
      
      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mz,Szy,Wzy);

      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  Wyz(j,k)=Wzy(k,j);
	}
      }
      

      Legolas::ATLASDenseMatrixMatrixProduct::apply(My,Wyz,Vyz);
      
      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  U[i][j][k]=Vyz(j,k);
	}
      }
    }

    for (int k=0 ; k< sizeZ ; k++){ ////Permutation en lecture!! 
      
      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){
	  Vxy(i,j)=U[i][j][k];
	}
      }
      
      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mx,Vxy,Uxy);

      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){

	  Uzxy[k][i][j]=Uxy(i,j);
	}
      }
    }


    for (int i=0 ; i< sizeX ; i++){ ////Permutation : on écrit dans le bon sens 
      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  U[i][j][k]=Uzxy[k][i][j];
	}
      }
    }
  }


  template <class DENSE_MATRIX_2D, class VECTOR_3D>
  static void applyXYZ_ZXY(const DENSE_MATRIX_2D & Mx,
			   const DENSE_MATRIX_2D & My,
			   const DENSE_MATRIX_2D & Mz,
			   const VECTOR_3D & S,
			   VECTOR_3D & U,
			   VECTOR_3D & Uzxy){
    
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

    DENSE_MATRIX_2D Szy(Legolas::MatrixShape<1>(sizeZ,sizeY));
    DENSE_MATRIX_2D Wzy(Legolas::MatrixShape<1>(sizeZ,sizeY));
    DENSE_MATRIX_2D Wyz(Legolas::MatrixShape<1>(sizeY,sizeZ));
    DENSE_MATRIX_2D Vyz(Legolas::MatrixShape<1>(sizeY,sizeZ));
    DENSE_MATRIX_2D Vxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    DENSE_MATRIX_2D Uxy(Legolas::MatrixShape<1>(sizeX,sizeY));
    
    for (int i=0 ; i< sizeX ; i++){
      
      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  Szy(k,j)=S[i][j][k];
	}
      }
      

      chronos.start();
      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mz,Szy,Wzy);
      chronos.stop();
      gemmTime+=chronos.get_elapsed_time_in_second();
      

      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  Wyz(j,k)=Wzy(k,j);
	}
      }

      chronos.start();
      Legolas::ATLASDenseMatrixMatrixProduct::apply(My,Wyz,Vyz);
      chronos.stop();
      gemmTime+=chronos.get_elapsed_time_in_second();
      
      for (int j=0 ; j< sizeY ; j++){
	for (int k=0 ; k< sizeZ ; k++){
	  U[i][j][k]=Vyz(j,k);
	}
      }
    }

    for (int k=0 ; k< sizeZ ; k++){ ////Permutation en lecture!! 
      
      chronos.start();
      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){
	  Vxy(i,j)=U[i][j][k];
	}
      }
      chronos.stop();
      permTime+=chronos.get_elapsed_time_in_second();
      
      chronos.start();
      Legolas::ATLASDenseMatrixMatrixProduct::apply(Mx,Vxy,Uxy);
      chronos.stop();
      gemmTime+=chronos.get_elapsed_time_in_second();

      for (int i=0 ; i< sizeX ; i++){ 
	for (int j=0 ; j< sizeY ; j++){

	  Uzxy[k][i][j]=Uxy(i,j);
	}
      }
    }

    chronos_tot.stop();
    double totTime=chronos_tot.get_elapsed_time_in_second();
    INFOS("totTime="<<totTime);
    INFOS("gemmTime="<<gemmTime<<" ("<<100.0*gemmTime/totTime<<"%)");
    INFOS("permTime="<<permTime<<" ("<<100.0*permTime/totTime<<"%)");
    
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


//   template <class DENSE_MATRIX_2D, class VECTOR_3D>
//   static void applyZXY_XYZ(const DENSE_MATRIX_2D & Mx,
// 			   const DENSE_MATRIX_2D & My,
// 			   const DENSE_MATRIX_2D & Mz,
// 			   const VECTOR_3D & Szxy,
// 			   VECTOR_3D & Uxyz){
    
//     // Mx Transposition
//     const int sizeX=Mx.nrows();
//     const int sizeY=My.nrows();
//     const int sizeZ=Mz.nrows();

//     DENSE_MATRIX_2D Sxy(Legolas::MatrixShape<1>(sizeX,sizeY));
//     DENSE_MATRIX_2D Wyx(Legolas::MatrixShape<1>(sizeY,sizeX));
//     DENSE_MATRIX_2D Vxy(Legolas::MatrixShape<1>(sizeX,sizeY));
//     DENSE_MATRIX_2D Uzy(Legolas::MatrixShape<1>(sizeZ,sizeY));


//     for (int k=0 ; k< sizeZ ; k++){ 
      
//       for (int i=0 ; i< sizeX ; i++){ 
// 	for (int j=0 ; j< sizeY ; j++){
// 	  Sxy(i,j)=S[k][i][j];
// 	}
//       }
      
//       Legolas::DenseMatrixMatrixProductDT::apply(My,Sxy,Wyx);
 //       Legolas::DenseMatrixMatrixProductDT::apply(Mx,Wyx,Vxy);
      
      
//       for (int j=0 ; j< sizeY ; j++){
// 	for (int k=0 ; k< sizeZ ; k++){
// 	  V[k][i][j]=Vxy(i,j);
// 	}
//       }
//     }
    
//     for (int i=0 ; i< sizeX ; i++){////Permutation en lecture!! 
      
//       for (int k=0 ; k< sizeZ ; k++){
// 	for (int j=0 ; j< sizeY ; j++){
// 	  Vzy(k,j)=V[k][i][j];
// 	}
//       }
      
//       Legolas::DenseMatrixMatrixProductDD::apply(Mz,Vzy,Uzy);
      
//       for (int j=0 ; j< sizeY ; j++){
// 	for (int k=0 ; k< sizeZ ; k++){
// 	  U[i][j][k]=Uzy(k,j);
// 	}
//       }
//     }
//   }
// };


// struct Tensorial3DProductV1{
  
//   template <class DENSE_MATRIX_2D, class VECTOR_3D>
//   static void applyXYZ_ZXY(const DENSE_MATRIX_2D & Mx,
// 			   const DENSE_MATRIX_2D & My,
// 			   const DENSE_MATRIX_2D & Mz,
// 			   const VECTOR_3D & Sxyz,
// 			   VECTOR_3D & Uzxy){
    
//     // Mx Transposition
//     const int sizeX=Mx.nrows();
//     const int sizeY=My.nrows();
//     const int sizeZ=Mz.nrows();

//     DENSE_MATRIX_2D Syz(Legolas::MatrixShape<1>(sizeY,sizeZ));
//     DENSE_MATRIX_2D Wzy(Legolas::MatrixShape<1>(sizeZ,sizeY));
//     DENSE_MATRIX_2D Vyz(Legolas::MatrixShape<1>(sizeY,sizeZ));
//     DENSE_MATRIX_2D Uxy(Legolas::MatrixShape<1>(sizeX,sizeY));

//     for (int i=0 ; i< sizeX ; i++){
      
//       for (int j=0 ; j< sizeY ; j++){
// 	for (int k=0 ; k< sizeZ ; k++){
// 	  Syz(j,k)=S[i][j][k];
// 	}
//       }
      
//       Legolas::DenseMatrixMatrixProductDT::apply(Mz,Syz,Wzy);
//       Legolas::DenseMatrixMatrixProductDT::apply(My,Wzy,Vyz);

//       for (int j=0 ; j< sizeY ; j++){
// 	for (int k=0 ; k< sizeZ ; k++){
// 	  V[i][j][k]=Vyz(j,k);
// 	}
//       }
//     }

//     for (int k=0 ; k< sizeZ ; k++){ ////Permutation en lecture!! 
      
//       for (int i=0 ; i< sizeX ; i++){ 
// 	for (int j=0 ; j< sizeY ; j++){
// 	  Vxy(i,j)=V[i][j][k];
// 	}
//       }
      
//       Legolas::DenseMatrixMatrixProductDD::apply(Mx,Vxy,Uxy);

//       for (int j=0 ; j< sizeY ; j++){
// 	for (int k=0 ; k< sizeZ ; k++){
// 	  U[k][i][j]=Uxy(i,j);
// 	}
//       }
//     }
//   }

//   template <class DENSE_MATRIX_2D, class VECTOR_3D>
//   static void applyZXY_XYZ(const DENSE_MATRIX_2D & Mx,
// 			   const DENSE_MATRIX_2D & My,
// 			   const DENSE_MATRIX_2D & Mz,
// 			   const VECTOR_3D & Szxy,
// 			   VECTOR_3D & Uxyz){
    
//     // Mx Transposition
//     const int sizeX=Mx.nrows();
//     const int sizeY=My.nrows();
//     const int sizeZ=Mz.nrows();

//     DENSE_MATRIX_2D Sxy(Legolas::MatrixShape<1>(sizeX,sizeY));
//     DENSE_MATRIX_2D Wyx(Legolas::MatrixShape<1>(sizeY,sizeX));
//     DENSE_MATRIX_2D Vxy(Legolas::MatrixShape<1>(sizeX,sizeY));
//     DENSE_MATRIX_2D Uzy(Legolas::MatrixShape<1>(sizeZ,sizeY));


//     for (int k=0 ; k< sizeZ ; k++){ 
      
//       for (int i=0 ; i< sizeX ; i++){ 
// 	for (int j=0 ; j< sizeY ; j++){
// 	  Sxy(i,j)=S[k][i][j];
// 	}
//       }
      
//       Legolas::DenseMatrixMatrixProductDT::apply(My,Sxy,Wyx);
//       Legolas::DenseMatrixMatrixProductDT::apply(Mx,Wyx,Vxy);
      
      
//       for (int j=0 ; j< sizeY ; j++){
// 	for (int k=0 ; k< sizeZ ; k++){
// 	  V[k][i][j]=Vxy(i,j);
// 	}
//       }
//     }
    
//     for (int i=0 ; i< sizeX ; i++){////Permutation en lecture!! 
      
//       for (int k=0 ; k< sizeZ ; k++){
// 	for (int j=0 ; j< sizeY ; j++){
// 	  Vzy(k,j)=V[k][i][j];
// 	}
//       }
      
//       Legolas::DenseMatrixMatrixProductDD::apply(Mz,Vzy,Uzy);
      
//       for (int j=0 ; j< sizeY ; j++){
// 	for (int k=0 ; k< sizeZ ; k++){
// 	  U[i][j][k]=Uzy(k,j);
// 	}
//       }
//     }
//   }
};




//////////////////////////////////////////////////////////////////////////////////:

template <class DENSE_MATRIX_2D, class VECTOR_3D>
void tensorial3DProductV2(const DENSE_MATRIX_2D & Mx,
			  const DENSE_MATRIX_2D & My,
			  const DENSE_MATRIX_2D & Mz,
			  const VECTOR_3D & S,
			  VECTOR_3D & U){
  
  // Mx Transposition
  const int size=Mx.nrows();
  
  DENSE_MATRIX_2D MUpp(Mx);
  DENSE_MATRIX_2D MUp(Mx);
  DENSE_MATRIX_2D MU(Mx);
  DENSE_MATRIX_2D MSXY(Mx);

  X86Timer chronos;

  double readSTime=0.0;
  double dgemmTime=0.0;
  double writeUTime=0.0;
  double readUTime=0.0;
  double permutationUTime=0.0;
  VECTOR_3D UP(S);
  
  for (int k=0 ; k< size ; k++){
    
    chronos.start();

    for (int i=0 ; i< size ; i++){
      for (int j=0 ; j< size ; j++){
	MSXY(i,j)=S[i][j][k];
      }
    }

    chronos.stop();
    readSTime+=chronos.get_elapsed_time_in_second();
   
    chronos.start();
    Legolas::ATLASDenseMatrixMatrixProduct::apply(Mx,MSXY,MUpp);
    Legolas::ATLASDenseMatrixMatrixProduct::apply(My,MUpp,MUp);
    chronos.stop();
    dgemmTime+=chronos.get_elapsed_time_in_second();
    
    chronos.start();
    for (int i=0 ; i< size ; i++){
      for (int j=0 ; j< size ; j++){
	UP[k][i][j]=MUp(i,j);
      }
    }
    chronos.stop();
    writeUTime+=chronos.get_elapsed_time_in_second();
  }

  
  for (int i=0 ; i< size ; i++){

    chronos.start();
    for (int k=0 ; k< size ; k++){
      for (int j=0 ; j< size ; j++){
	MUp(k,j)=UP[k][i][j];
      }
    }
    chronos.stop();
    readUTime+=chronos.get_elapsed_time_in_second();
    
    chronos.start();
    Legolas::ATLASDenseMatrixMatrixProduct::apply(Mz,MUp,MU);
    chronos.stop();
    dgemmTime+=chronos.get_elapsed_time_in_second();

    chronos.start();
    for (int j=0 ; j< size ; j++){
      for (int k=0 ; k< size ; k++){
 	U[i][j][k]=MU(j,k);
      }
    }
    chronos.stop();
    permutationUTime+=chronos.get_elapsed_time_in_second();
  }


  INFOS("dgemmTime="<<dgemmTime);
  INFOS("permutateReadUTime="<<readUTime);
  INFOS("permutateWriteUTime="<<permutationUTime);

  INFOS("readSTime="<<readSTime);
  INFOS("writeUTime="<<writeUTime);
  

  
}

#endif  
			
