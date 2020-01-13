/**
 * project  DESCARTES
 *
 * @file     helper.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_HELPER_HXX__
#define __LEGOLAS_HELPER_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  template <class MATRIX>
  void display(const MATRIX & A){
    INFOS("A.nrows()="<<A.nrows());
    INFOS("A.ncols()="<<A.ncols());

    for (int i=0 ; i < A.nrows() ; i++ ){
      for (int j=0 ; j < A.ncols() ; j++ ){
	INFOS("A("<<i<<","<<j<<")="<<A(i,j));
      }
    }
    return ;
  }



  template <class MATRIX>
  void display_2D(const MATRIX & A){

    for (int i=0 ; i < A.nrows() ; i++ ){
      for (int j=0 ; j < A.ncols() ; j++ ){

	for (int k=0 ; k < A(i,j).nrows() ; k++ ){
	  for (int l=0 ; l < A(i,j).ncols() ; l++ ){
	    INFOS("A2D("<<i<<","<<j<<")("<<k<<","<<l<<")="<<A(i,j)(k,l));
	  }
	}
      }
    }
    return ;
  }

  template <class MATRIX>
  void display_2D_DR(const MATRIX & A){

    for (int i=0 ; i < A.nrows() ; i++ ){
      for (int j=0 ; j < A.ncols() ; j++ ){

	for (int k=0 ; k < A(i,j).nrows() ; k++ ){
	  for (int l=0 ; l < A(i,j).ncols() ; l++ ){
	    INFOS("A2D("<<i<<","<<j<<")("<<k<<","<<l<<")="<<A(i,j)(k,l));
	  }
	}
      }
    }
    return ;
  }

  template <class MATRIX_A,class MATRIX_B,class MATRIX_C>
  void display_2D_3M(const MATRIX_A & A, const MATRIX_B & B, const MATRIX_C & C){
  
    for (int i=0 ; i < A.nrows() ; i++ ){
      for (int j=0 ; j < A.ncols() ; j++ ){
      
	for (int k=0 ; k < A(i,j).nrows() ; k++ ){
	  for (int l=0 ; l < A(i,j).ncols() ; l++ ){
	    INFOS("A2D("<<i<<","<<j<<")("<<k<<","<<l<<")="<<A(i,j)(k,l)<<"="<<B(i,j)(k,l)<<"="<<C(i,j)(k,l));
	  }
	}
      }
    }
    return ;
  }



  template <class MATRIX>
  double sum(const MATRIX & A){
    double result=0.0;

    for (int i=0 ; i < A.nrows() ; i++ ){
      for (int j=0 ; j < A.ncols() ; j++ ){
	result+=A(i,j);
      }
    }
    return result;
  }

  template <class MATMAT>
  double sumsum(const MATMAT & A){
    double result=0.0;

    for (int i=0 ; i < A.nrows() ; i++ ){
      for (int j=0 ; j < A.ncols() ; j++ ){
	result+=sum(A(i,j));
      }
    }  
    return result;
  }




  template <class BANDED_MATRIX>
  double bandedSum(const BANDED_MATRIX & A){
    double result=0.0;

    int linf=A.linf();
    int lsup=A.lsup();


    result=A.bandedGetElement(0,0);
    result+=A.bandedGetElement(0,1);

    int nrows=A.nrows();
    for (int i=1 ; i < nrows-1 ; i++ ){
      for (int j=i-linf ; j < i+lsup+1 ; j++ ){
	result+=A.bandedGetElement(i,j);
      }
    }
    result+=A.bandedGetElement(nrows-1,nrows-2);
    result+=A.bandedGetElement(nrows-1,nrows-1);
    return result;
  }






  template <class VECTOR_IN, class VECTOR_OUT>
  void copyVector2D(const VECTOR_IN & in , VECTOR_OUT & out){
    out.resize(in.size());
    for (int i=0 ; i < in.size() ; i++){
      out[i].resize(in[i].size());
      for (int j=0 ; j < out.size() ; j++){
	out[i][j]=in[i][j];
      }
    }
  }  

  template <class MATRIX_IN, class VECTOR_OUT>
  void copyMatrix1D(const MATRIX_IN & in , VECTOR_OUT & out){
    out.resize(in.nrows());
    for (int i=0 ; i < out.size() ; i++){
      out[i].resize(in.ncols());
      for (int j=0 ; j < out[i].size() ; j++){
	out[i][j]=in(i,j);
      }
    }
  }  


  template <class MATRIX_IN, class VECTOR_OUT>
  void copyMatrix2D(const MATRIX_IN & in , VECTOR_OUT & out){
    out.resize(in.nrows());
    for (int i=0 ; i < out.size() ; i++){
      out[i].resize(in.ncols());
      for (int j=0 ; j < out[i].size() ; j++){
	copyMatrix1D(in(i,j),out[i][j]);
      }
    }
  }  



  // Y=A*X
  template <class MATRIX, class VECTOR>
  void MVP(const MATRIX & A, const VECTOR & X, VECTOR & Y){

    int size_out=X.size();
    int size_in=X[0].size();
  
    for (int i=0; i< size_out ; i++){
    
      for (int j=0; j< size_out ; j++){
      
	//s+=A[i][j]*X[j];
      
	for (int k=0; k< size_in ; k++){

	  double ss=0.0;

	  for (int l=0; l< size_in ; l++){

	    ss+=A[i][j][k][l]*X[j][l];

	  }

	  Y[i][k]=ss;
	}
      
      }
    
    }
  
    return ;
  }


  template <class VECTOR>
  void vector_display(const VECTOR & V){
    for (int i=0 ; i < V.size() ; i++ ){
      INFOS("V["<<i<<"]="<<V[i]);
    }
    return ;
  }

  template <class VECTOR>
  void vector_display_2D(const VECTOR & V){
    for (int i=0 ; i < V.size() ; i++ ){
      for (int j=0 ; j < V[i].size() ; j++ ){
	INFOS("V["<<i<<"]["<<j<<"]="<<V[i][j]);
      }
    }
    return ;
  }

  template <class VECTOR>
  typename VECTOR::RealType scalaire(const VECTOR & V){
    typename VECTOR::RealType result=VECTOR::dot(V,V);
    return result;
  }
  

}

#endif
