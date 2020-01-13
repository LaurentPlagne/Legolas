#ifndef __TBSMLDLTFACTORIZE_HXX_
#define __TBSMLDLTFACTORIZE_HXX_

#include <math.h>



struct TBSMLDLTFactorize{

  // LP: basic LDLT factorization. from Lascaux et Théodore 2eme edition (p219)
  // LP: This implementation is easy to read but could be
  // LP: much faster with specialized routines for statically defined halfbandwidth  

  template <class TBSM>
  static inline void applyLine(const int & hbw,
			       const int & nrows,
			       const int & i, TBSM & matrix){
    typedef typename TBSM::RealType RealType;
    //    const int hbw1=hbw+1;
    
    const int minj=std::max(i-hbw,0);
      
    for (int j=minj ; j < i ; j++){
      
      //      RealType s= matrix.lowerBandedGetElement(j,i);
      
      int mink1=std::max(i-hbw,0);
      int mink2=std::max(j-hbw,0);
	
      int mink=std::max(mink1,mink2);
	
      for (int k=mink ; k < j ; k++){
	matrix.lowerBandedGetElement(i,j)-=matrix.lowerBandedGetElement(i,k)*matrix.lowerBandedGetElement(j,k);
      }
    }

    for (int j=minj ; j < i ; j++){
      
      //      RealType s= matrix.lowerBandedGetElement(i,j)/matrix.lowerBandedGetElement(j,j);
      RealType s= matrix.lowerBandedGetElement(i,j)*matrix.lowerBandedGetElement(j,j);
      matrix.lowerBandedGetElement(i,i)-=s*matrix.lowerBandedGetElement(i,j);
      matrix.lowerBandedGetElement(i,j)=s;
    }

    matrix.lowerBandedGetElement(i,i)=1.0/ matrix.lowerBandedGetElement(i,i);


  }


  //i>=hbw
  template <class TBSM>
  static inline void applyLineCenter(const int & hbw,
				     const int & nrows,
				     const int & i, TBSM & matrix){
    typedef typename TBSM::RealType RealType;

    for (int j=i-hbw ; j < i ; j++){
      
      int mink=i-hbw;
	
      for (int k=mink ; k < j ; k++){
	matrix.lowerBandedGetElement(i,j)-=matrix.lowerBandedGetElement(i,k)*matrix.lowerBandedGetElement(j,k);
      }
    }
    
    for (int j=i-hbw ; j < i ; j++){
      
      RealType s= matrix.lowerBandedGetElement(i,j)*matrix.lowerBandedGetElement(j,j);
      matrix.lowerBandedGetElement(i,i)-=s*matrix.lowerBandedGetElement(i,j);
      matrix.lowerBandedGetElement(i,j)=s;
    }
    matrix.lowerBandedGetElement(i,i)=1.0/ matrix.lowerBandedGetElement(i,i);
  }


  
  template <class TBSM>
  static void apply(TBSM & matrix){
    
    typedef typename TBSM::RealType RealType;
    const int hbw=matrix.getHalfBandwidth();
    //    const int hbw1=hbw+1;
    const int nrows=matrix.nrows();
    
    for (int i=0 ; i < hbw ; i++){
      applyLine(hbw,nrows,i,matrix);
    }
    for (int i=hbw ; i < nrows ; i++){
      applyLineCenter(hbw,nrows,i,matrix);
      //applyLineCenterRT0(nrows,i,matrix);
    }
  }
  
};

#endif

    
