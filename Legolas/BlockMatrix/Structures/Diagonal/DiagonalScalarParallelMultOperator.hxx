#ifndef __DIAGONALSCALARPARALLELMULTOPERATOR_HXX__
#define __DIAGONALSCALARPARALLELMULTOPERATOR_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixMultOperator.hxx"

namespace Legolas{

  template <class SCALAR_MATRIX, class V1D>
  class AddMultLineOperator{
    const SCALAR_MATRIX & A_;
    const double & a_;
    const V1D & X_; 
    V1D & Y_;
  public:
    AddMultLineOperator(const SCALAR_MATRIX & A,
			const double & a,
			const V1D & X, V1D & Y):A_(A),a_(a),X_(X),Y_(Y){}
    
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      
      for (int i=r.begin() ; i!=r.end() ; i++){
	Y_[i]+=a_*A_.diagonalGetElement(i)*X_[i];
      }
    }
  };

  template <class SCALAR_MATRIX, class V1D>
  class MultLineOperator{
    const SCALAR_MATRIX & A_;
    const V1D & X_; 
    V1D & Y_;
  public:
    MultLineOperator(const SCALAR_MATRIX & A,
		     const V1D & X, V1D & Y):A_(A),X_(X),Y_(Y){}
    
    inline void operator()(const my_tbb::blocked_range<int> & r) const {
      
      for (int i=r.begin() ; i!=r.end() ; i++){
	Y_[i]=A_.diagonalGetElement(i)*X_[i];
      }
    }
  };




  struct DiagonalScalarParallelMultOperator{
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixMultOperator<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:
      
      inline std::string name( void ) const { return "DiagonalScalarParallelMultOperator" ;}

      virtual VirtualMultOperator * clone( void ) const {
	return new Engine(*this);
      }
      

      
      //Y+=a*A*X
      void addMult(const SCALAR_MATRIX & A, const double & a, const V1D & X, V1D & Y){
	
	my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows(),100),AddMultLineOperator<SCALAR_MATRIX,V1D>(A,a,X,Y));
	
	// const int n=A.nrows();
	// RealType factor(a);
	// for (int i=0 ; i < n ; i++ ){
	//   Y[i]+=factor*A.diagonalGetElement(i)*X[i];
	// }
	}
      
      //Y=A*X
      void mult(const SCALAR_MATRIX & A, const V1D & X, V1D & Y){
	
	my_tbb::parallel_for(my_tbb::blocked_range<int>(0,A.nrows(),100),MultLineOperator<SCALAR_MATRIX,V1D>(A,X,Y));
	
	// const int n=A.nrows();
	// for (int i=0 ; i < n ; i++ ){
	//   Y[i]=A.diagonalGetElement(i)*X[i];
	// }
      }
    };
  };
  
}

#endif
    
