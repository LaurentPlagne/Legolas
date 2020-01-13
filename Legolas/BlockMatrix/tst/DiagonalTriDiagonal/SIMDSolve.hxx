#pragma once
#include <Eigen/Core>
#include "RealType.hxx"
#include "MyTriDiagonalMatrix.hxx"
#include "my_tbb_parallel_for.hxx"
#include "TBBSSESolverOperator.hxx"
#include "TBBSSESolverOperatorBis.hxx"

class SIMDSolve{

  static const int blockSize=4;

  typedef Legolas::MultiVector<RealType,1> V1D;
  
  typedef Eigen::Array<RealType, blockSize, 1> BlockArray;
  typedef Eigen::Map<BlockArray,Eigen::Aligned>  BlockArrayView;
  typedef Eigen::Map<const BlockArray,Eigen::Aligned>  ConstBlockArrayView;

  typedef Legolas::MultiVector<RealType,2>  V2D;

  V2D  XI_;
  V2D  BI_;
  V2D  SI_;

  V2D  DI_;  
  V2D  LI_;
  V2D  UI_;

  Legolas::MultiVector<RealType,1>  S_;

  int s1_,s2_;


public:
  SIMDSolve(const MyDiagonalBlockMatrix & A):XI_(),BI_(),DI_(),LI_(),UI_(),S_(),s1_(),s2_(){
    
    s1_=A.nrows();
    const MyTriDiagonalMatrix<RealType> & A00=static_cast<const  MyTriDiagonalMatrix<RealType> & >(A.diagonalGetElement(0));
    s2_=A00.nrows();

    Legolas::MultiVector<RealType,2>::Shape  shape(s1_/blockSize,s2_*blockSize);

    XI_.reshape(shape);    
    BI_.reshape(shape);    
    SI_.reshape(shape);    

    DI_.reshape(shape);    
    LI_.reshape(shape);    
    UI_.reshape(shape);    
    
    S_.reshape(s2_*blockSize);    
    
    for (int i=0 ; i<s1_/blockSize ; i++){
      for (int j=0 ; j<s2_ ; j++){
	for (int k=0 ; k<blockSize ; k++){
	  
	  const int newI=i;
	  const int newJ=blockSize*j+k;
	  const int oldI=blockSize*i+k;
	  const int oldJ=j;
	  
	  const MyTriDiagonalMatrix<RealType> & Aii=static_cast<const  MyTriDiagonalMatrix<RealType> & >(A.diagonalGetElement(oldI));
	  
	  DI_[newI][newJ]=Aii.diagonalGetElement(oldJ);
	  LI_[newI][newJ]=Aii.lowerDiagonalGetElement(oldJ);
	  UI_[newI][newJ]=Aii.upperDiagonalGetElement(oldJ);
	  
	}
      }
    }
  }

  void updateB(const V2D & B){

    for (int i=0 ; i<s1_/blockSize ; i++){
      for (int j=0 ; j<s2_ ; j++){
	for (int k=0 ; k<blockSize ; k++){
	  
	  const int newI=i;
	  const int newJ=blockSize*j+k;
	  const int oldI=blockSize*i+k;
	  const int oldJ=j;

	  BI_[newI][newJ]=B[oldI][oldJ];
	}
      }
    }
  }

  void updateB(RealType value){
    BI_=value;
  }


  void updateX(V2D & X) const {

    for (int i=0 ; i<s1_/blockSize ; i++){
      for (int j=0 ; j<s2_ ; j++){
	for (int k=0 ; k<blockSize ; k++){
	  
	  const int newI=i;
	  const int newJ=blockSize*j+k;
	  const int oldI=blockSize*i+k;
	  const int oldJ=j;

	  X[oldI][oldJ]=XI_[newI][newJ];
	}
      }
    }
  }
  
  
  void solve( void ){

    for (int b=0 ; b<s1_/blockSize ; b++){
	  
      const V1D & D=DI_[b];
      const V1D & U=UI_[b];
      const V1D & L=LI_[b];
	
      const V1D & B=BI_[b];
	
      V1D & X=XI_[b];
	
      ConstBlockArrayView D0(&D[0]);
      BlockArray s(D0);
	
      BlockArrayView X0(&X[0]);
      ConstBlockArrayView B0(&B[0]);
	
      //	  X0=B0/s;
      X0=B0;
      X0/=s;
	
	
      for (int i=1 ; i < s2_ ; i++ ){
	  
	const int I=blockSize*i;
	  
	//S_[i]=A.upperDiagonalGetElement(i-1)/s;
	BlockArrayView Si(&S_[I]);
	Si=ConstBlockArrayView(&U[I-blockSize]);
	Si/=s;
	  
	//	    Si=ConstBlockArrayView(&U[I])/s;
	  
	ConstBlockArrayView Li(&L[I]);
	  
	s=ConstBlockArrayView(&D[I])-Li*Si;
	  
	BlockArrayView Xi(&X[I]);
	ConstBlockArrayView Xim1(&X[I-blockSize]);
	ConstBlockArrayView Bi(&B[I]);
	Xi=Bi;
	Xi-=Li*Xim1;
	Xi/=s;
	  
      }
	
      for (int i=(s2_-2) ; i >= 0 ; i-- ){
	  
	const int I=blockSize*i;
	//X[i]-=S_[i+1]*X[i+1];
	BlockArrayView Xi(&X[I]);
	ConstBlockArrayView Sip1(&S_[I+blockSize]);
	ConstBlockArrayView Xip1(&X[I+blockSize]);
	Xi-=Sip1*Xip1;
      }
    }
      
  }


  void parallelSolve( void ){
    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,s1_/blockSize),TBBSSESolverOperator(DI_,LI_,UI_,BI_,XI_));
    //    my_tbb::parallel_for(my_tbb::blocked_range<int>(0,s1_/blockSize),TBBSSESolverOperatorBis(DI_,LI_,UI_,BI_,SI_,XI_));
  }

};
    
    
