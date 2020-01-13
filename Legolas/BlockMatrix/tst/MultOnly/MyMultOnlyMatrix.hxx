#ifndef __LEGOLAS_MYMULTONLYMATRIX_HXX__
#define __LEGOLAS_MYMULTONLYMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/MultOnly/MultOnlyScalarMatrix.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixMultOperator.hxx"


class MyMultOperator{
public:
  
  template <class SCALAR_MATRIX>
  class Engine : public Legolas::ScalarMatrixMultOperator<SCALAR_MATRIX>{
  public:
    typedef typename SCALAR_MATRIX::RealType RealType;
    typedef Legolas::MultiVector<RealType,1> V1D;

    
    virtual inline std::string name( void ) const { return "MyMultOperator" ;}
    
    virtual Legolas::VirtualMultOperator * clone( void ) const {
      return new Engine(*this);
    }
    
    //Y+=a*A*X
    void addMult(const SCALAR_MATRIX & A, const double & a, const V1D & X, V1D & Y){
      
      const int n=A.nrows();
      const RealType factor=a;
      const RealType two=A.factor();

      Y[0]+=factor*(two*X[0]-X[1]);
      for (int i=1 ; i < n-1 ; i++ ){
      	Y[i]+=factor*(two*X[i]-X[i-1]-X[i+1]);
      }
      Y[n-1]+=factor*(two*X[n-1]-X[n-2]);
      
    }
    
    //Y+=a*A*X
    inline void mult(const SCALAR_MATRIX & A, const V1D & X, V1D & Y){

      const int n=A.nrows();
      const RealType two=A.factor();

      Y[0]=two*X[0]-X[1];
      for (int i=1 ; i < n-1 ; i++ ){
      	Y[i]=two*X[i]-X[i-1]-X[i+1];
      }
      Y[n-1]=two*X[n-1]-X[n-2];
      
    }
  };
};

  
  




template <class SCALAR>
class MyMultOnlyMatrix : public Legolas::MultOnlyScalarMatrix<SCALAR> {
public:
  typedef SCALAR RealType;
  typedef Legolas::MultOnlyScalarMatrix<SCALAR> Base;
  typedef Legolas::MultiVector<RealType,1> V1D;
  int factor_;


  
  std::string name( void ) const { return "MyMultOnlyMatrix" ;}
  
  int factor( void ) const {return factor_;}
  
  MyMultOnlyMatrix(const Legolas::MatrixShape<1> & ms, int factor):Base(ms),factor_(factor){
    
    this->setVirtualMultOperatorPtr(new MyMultOperator::template Engine<MyMultOnlyMatrix>());
    
  }
};
  
#endif
    
