#ifndef __SPACEMATRIXTEST__
#define __SPACEMATRIXTEST__

#include "UTILITES.hxx" 

#include "SpaceVectorPrivateData.hxx"
#include "SpaceSourceVectorDefinition.hxx"
#include "SpaceExactVectorDefinition.hxx"
#include "SpaceTemperatureVectorDefinition.hxx"

#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "DSpaceMatrixDefinition.hxx"
#include "BSpaceMatrixDefinition.hxx"

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"

#include "GenericVector.hxx"

template <class MATRIX_INTERFACE, class DATA, class VECTOR>
void testInverse(const VECTOR & B, const VECTOR & E, const DATA & data, VECTOR & X, VECTOR & R){

  typedef typename MATRIX_INTERFACE::Matrix  Matrix;
  
  
  Matrix A(data);
  
  int i=1;
  int j=1;
  
  INFOS("A("<<i<<","<<j<<")="<<A(i,j));
  
  Legolas::solve(A,X,B);
  R=A*X;
  
  R-=B; 
  
  INFOS("Residual R=AX-B  norme="<<(VECTOR::dot(R,R)/VECTOR::dot(X,X)));

};



template <class REAL_TYPE>
class SpaceMatrixTest{
public:

  typedef REAL_TYPE RealType;

  SpaceMatrixTest( void ){
    MESSAGE("SpaceMatrixTest Ctor");
  }

  void apply( RealType deltaT ) const{

    
    typedef typename GenericVector< SpaceSourceVectorDefinition<RealType> >::Vector SpaceVector;
    SpaceVector X,B,E,R;
    
    SpaceVectorPrivateData<RealType> vectorData(deltaT,0) ;
    
    GenericVector< SpaceSourceVectorDefinition<RealType>      >::initialize(B,vectorData);
    GenericVector< SpaceExactVectorDefinition<RealType>       >::initialize(E,vectorData);
    GenericVector< SpaceTemperatureVectorDefinition<RealType> >::initialize(X,vectorData);
    GenericVector< SpaceTemperatureVectorDefinition<RealType> >::initialize(R,vectorData);

    {

      typedef DSpaceMatrixDefinition<RealType> MatrixDefinition;
      typedef typename MatrixDefinition::Data   Data;
      Data data(deltaT);

    
      {
	
	INFOS("TEST #1 default options (virtual)");

	typedef InputMatrixOptions<Virtual> MatrixOptions;
	typedef GenericMatrixInterface< MatrixDefinition, MatrixOptions, RealElementInterface<RealType> > MatrixInterface;
	
	testInverse<MatrixInterface,Data,SpaceVector>(B,E,data,X,R);
	
      }
      
      {
	
	INFOS("TEST #2 default options (Actual)");

	typedef InputMatrixOptions<Actual> MatrixOptions;
	typedef GenericMatrixInterface< MatrixDefinition, MatrixOptions, RealElementInterface<RealType> > MatrixInterface;
	
	testInverse<MatrixInterface,Data,SpaceVector>(B,E,data,X,R);
	
      }
      
      {
	
	INFOS("TEST #3 default options explicite (Virtual)");

	SparseGaussSeidel::_epsilon=1.e-8;
	
	typedef InputMatrixOptions<Actual,SparseGaussSeidel,SparseMatrixVectorProduct,STLVectorContainer,TriDiagonalMatrixContainer,TriDiagonal> MatrixOptions;
	
	typedef GenericMatrixInterface< MatrixDefinition, MatrixOptions, RealElementInterface<RealType> > MatrixInterface;
	testInverse<MatrixInterface,Data,SpaceVector>(B,E,data,X,R);
	
      }
      
      
      {

	INFOS("TEST #4 Tridiagonal LU solver options par default (virtual)");
	
	typedef InputMatrixOptions<Virtual,TriDiagonalMatrixInversionByLUDecomposition,SparseMatrixVectorProduct,STLVectorContainer,TriDiagonalMatrixContainer,TriDiagonal> MatrixOptions;
	
	typedef GenericMatrixInterface< MatrixDefinition, MatrixOptions, RealElementInterface<RealType> > MatrixInterface;
	testInverse<MatrixInterface,Data,SpaceVector>(B,E,data,X,R);
	
      }

      {

	INFOS("TEST #4.5 DensGaussSeidel solver options par default (actual)");
	
	typedef InputMatrixOptions<Virtual,SparseGaussSeidel,SparseMatrixVectorProduct,STLVectorContainer,TriDiagonalMatrixContainer,TriDiagonal> MatrixOptions;
	
	typedef GenericMatrixInterface< MatrixDefinition, MatrixOptions, RealElementInterface<RealType> > MatrixInterface;
	testInverse<MatrixInterface,Data,SpaceVector>(B,E,data,X,R);
	
      }
      
    }

    {
    
      typedef BSpaceMatrixDefinition<RealType>  MatrixDefinition;
      typedef typename MatrixDefinition::Data   Data;
      Data data(deltaT);

      {
	
	INFOS("TEST #5 Matrice B (virtual)");

	typedef InputMatrixOptions<Virtual> MatrixOptions;
	typedef GenericMatrixInterface< MatrixDefinition, MatrixOptions, RealElementInterface<RealType> > MatrixInterface;
	
	testInverse<MatrixInterface,Data,SpaceVector>(B,E,data,X,R);
	
      }

    }

  }



};

#endif

      
  

  
