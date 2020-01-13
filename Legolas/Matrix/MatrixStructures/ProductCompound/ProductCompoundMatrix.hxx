/**
 * project  DESCARTES
 *
 * @file     PRODUCTCOMPOUNDMatrix.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_PRODUCTCOMPOUNDMATRIX_HXX__
#define __LEGOLAS_PRODUCTCOMPOUNDMATRIX_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Matrix/StorageModeTraits/ConstRefTraits.hxx"
#include "Legolas/Matrix/MatrixStructures/Helper/AllFilled.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/MultiLevelSparseMatrixInterface.hxx"
#include "Legolas/Matrix/PlusEqualProduct.hxx"

#include <set>
#include <map>
#include <cassert>


namespace Legolas{

  template <class L_MATRIX_INTERFACE, 
	    class R_MATRIX_INTERFACE, 
	    class PC_INV,
	    class PC_MULT>
  class ProductCompoundMatrix:  public BaseMatrix< ProductCompoundMatrix<L_MATRIX_INTERFACE,R_MATRIX_INTERFACE,PC_INV,PC_MULT> >,
				public MatrixBasicData<typename L_MATRIX_INTERFACE::Matrix::MatrixElementInterface::DataDriver>
  {
  private:
    typedef BaseMatrix< ProductCompoundMatrix<L_MATRIX_INTERFACE,R_MATRIX_INTERFACE,PC_INV,PC_MULT> > Base1;
    typedef MatrixBasicData<typename L_MATRIX_INTERFACE::Matrix::MatrixElementInterface::DataDriver> Base2;
  public:
  
    typedef PC_MULT MatrixVectorProduct;
    typedef PC_INV  MatrixVectorInverse;

    typedef Actual StorageMode;

    typedef typename ConstRefTraits<ProductCompoundMatrix,StorageMode>::Type StoreType;

    typedef L_MATRIX_INTERFACE LMatrixInterface;
    typedef R_MATRIX_INTERFACE RMatrixInterface;

    typedef typename L_MATRIX_INTERFACE::Matrix::VectorOperand VectorOperand;
    typedef typename MatrixVectorInverse::template Engine<ProductCompoundMatrix,VectorOperand,VectorOperand > Solver;

    
  private:

    typedef typename LMatrixInterface::MatrixDefinition::Data VirtualLData;
    typedef typename RMatrixInterface::MatrixDefinition::Data VirtualRData;

  public:

    struct DataDriver{
    
      static std::string name( void ) {return "ProductCompound<>::DataDriver";}

      typedef ProductCompoundMatrix Data;

      static const int level=LMatrixInterface::Matrix::level;
      typedef typename LMatrixInterface::DataDriver::RealType      RealType;
    
      template< class INPUT_PC_MATRIX >
      static inline void copy(const INPUT_PC_MATRIX & inputMatrix, Data & matrix){
	matrix=inputMatrix;
      }
    };

    typedef typename DataDriver::RealType RealType;

    // Pour compatibilité avec le concept MatrixInterface
    typedef typename L_MATRIX_INTERFACE::Matrix::MatrixElementInterface MatrixElementInterface;

    class MatrixDefinition{
    public:
      
      class Data{
      public:
	Data(const VirtualLData & vld,const VirtualRData & vrd):vld_(vld),
								vrd_(vrd){
	}

	const VirtualLData getVirtualLData() const {return vld_;}
	const VirtualRData getVirtualRData() const {return vrd_;}
	
      private:
	const VirtualLData & vld_;
	const VirtualLData & vrd_;
      };
    };
	
    //    ProductCompoundMatrix( void ):L_(VirtualLData()),R_( VirtualRData()){}
    
    ProductCompoundMatrix(const VirtualLData & vld,const VirtualRData & vrd):L_(vld),
									     R_(vrd){

      assert(L_.getColShape()==R_.getRowShape());

      this->setMaxIteration()=this->L().getMaxIteration();
      this->setEpsilon()=this->L().getEpsilon();
      
      this->setRowShape()=this->L().getRowShape();
      this->setColShape()=this->R().getColShape();
 
    }

    ProductCompoundMatrix(const typename MatrixDefinition::Data & vdata):L_(vdata.getVirtualLData()),
									 R_(vdata.getVirtualRData()){

      assert(L_.getColShape()==R_.getRowShape());

      this->setMaxIteration()=this->L().getMaxIteration();
      this->setEpsilon()=this->L().getEpsilon();
      
      this->setRowShape()=this->L().getRowShape();
      this->setColShape()=this->R().getColShape();
    }

    
    typename LMatrixInterface::View      L( void )       {return L_ ; };
    typename LMatrixInterface::ConstView L( void ) const {return L_ ; };
    typename RMatrixInterface::View      R( void )       {return R_ ; };
    typename RMatrixInterface::ConstView R( void ) const {return R_ ; };
	


    std::string getStructureName( void ) const {
      typedef typename LMatrixInterface::DataDriver LDataDriver;    
      typedef typename RMatrixInterface::DataDriver RDataDriver;    
      std::string result="ProductCompoundMatrix (L=";
      result+=LDataDriver::name();
      result+=" , R=";
      result+=RDataDriver::name();
      result+=")";
      return result;
    }

    static const int level=LMatrixInterface::Matrix::level;


    class FilledRows{
    public:

      class FilledCols{
      public:
	FilledCols(){};
	
	void insert(int colIndex){
	  colIndexes_.insert(colIndex);
	}
	int begin() const {
	  assert(colIndexes_.size()!=0);
	  return *(colIndexes_.begin());
	}

	int end() const { // "past the end" mais au sens des indices (pas des itérateurs)
	  assert(colIndexes_.size()!=0);
	  return *(colIndexes_.rbegin())+1;
	}

	void next(int & index){
	  assert(colIndexes_.size()!=0);
	  std::set<int>::const_iterator iter=colIndexes_.find(index);

	  iter++;

	  if (iter==colIndexes_.end()){ //index == last element
	    index++;
	  }
	  else{
	    index=(*iter);
	  }
	  
	}
	
      private:
	std::set<int> colIndexes_;
      };

      std::map<int,FilledCols> filledRows_;
      
    public:
      
      template <class PC_MATRIX>
      FilledRows( const PC_MATRIX & matrix ){


	typename LMatrixInterface::Matrix::FilledRows lRows(matrix.L());

	for (int i=lRows.begin(); i< lRows.end() ; lRows.next(i)){
	  
	  bool aiRowIsNull=true;

	  FilledCols fcir;

	  for (int j=0; j< matrix.R().ncols() ; j++){

	    bool aijIsNull=true;

	    for (int k=0 ; k<  matrix.L().ncols() ; k++){
	      if (!(zeroElement(i,k,matrix.L()))&&(!zeroElement(k,j,matrix.R()))){
		aijIsNull=false;
	      }
	    }

	    if (!aijIsNull){
	      fcir.insert(j);
	      aiRowIsNull=false;
	    }
	  }

	  if (!aiRowIsNull){
	    filledRows_[i]=fcir;
	  }
	}
      }



// 	typename LMatrixInterface::Matrix::FilledRows lRows(matrix.L());
// 	typename RMatrixInterface::Matrix::FilledCols rCols(matrix.R());

// 	for (int i=lRows.begin(); i< lRows.end() ; lRows.next(i)){

// 	  typename LMatrixInterface::Matrix::FilledRows::FilledCols lCols=lRows.filledColsInRow(i);

// 	  for (int k=lCols.begin(); k< lCols.end() ; lCols.next(k)){
	    
// 	    typename RMatrixInterface::Matrix::FilledRows rRows(matrix.R());
	    
// 	    int kr=rRows.begin();
// 	    while ( kr < k ) rRows.next(kr);

// 	    if (kr==k){//i^th row of LR is non empty

// 	      FilledCols fcir;

// 	      typename RMatrixInterface::Matrix::FilledRows::FilledCols rCols=rRows.filledColsInRow(k);
	      
// 	      for (int j=rCols.begin(); j< rCols.end() ; rCols.next(j)){
// 		fcir.insert(j);
// 	      }

// 	      filledRows_[i]=fcir;
// 	    }
// 	  }
// 	}
      //      }

      FilledCols filledColsInRow( int rowIndex ) {
	return filledRows_[rowIndex];
      }
      
      int begin( void ) const{
	return (*filledRows_.begin()).first;
      }

      // "past the end" mais au sens des indices (pas des itérateurs)
      int end( void ) const{
	//	return (*filledRows_.end()-1).first+1;

	typename std::map<int,FilledCols>::const_reverse_iterator last=filledRows_.rbegin();
	return (*last).first+1;
      }

      void next( int & rowIndex){
	typename std::map<int,FilledCols>::const_iterator iter=filledRows_.find(rowIndex);
	iter++;
	if (iter!=filledRows_.end()){
	  rowIndex=(*iter).first;
	}
	else{
	  rowIndex++;
	}
	//	rowIndex=*(filledRows_.find(rowIndex)++);
      }
	
    };//end of class ProductCompund::FilledRows

    template <class T>
    static inline void initializeFloatElementToZero( T & value){};
    static inline void initializeFloatElementToZero( double & value){value=0.0;}
    static inline void initializeFloatElementToZero( float & value){value=0.0;}


    typedef typename Legolas::MultiLevelSparseMatrixInterface<RealType,level>::Matrix::MatrixElementInterface::Copy ElementType;

    ElementType sparseGetElement(int i, int j) const{

      const  ProductCompoundMatrix & block=*this;
      ElementType aij;
      initializeFloatElementToZero(aij);
    
      typename LMatrixInterface::ConstView l=block.L();
      typename RMatrixInterface::ConstView r=block.R();
    
      for (int k=0 ; k< l.ncols() ; k++){
	if (!(zeroElement(i,k,l))&&(!zeroElement(k,j,r))){
	  
	  r.sparseGetElement(k,j);
	  l.sparseGetElement(i,k);
	  
	  Legolas::plusEqualProduct(l.sparseGetElement(i,k),r.sparseGetElement(k,j),aij);
	}
      }

      return aij;
    }

    typedef ElementType ConstGetElement;


    void iterateOverBlocksMethod(Legolas::MatrixStream & matrixStream) const {
      Legolas::IterateOverBlocks::apply((*this),matrixStream);
    }


//     template <class TRAVERSAL_POLICY>
//     struct ElementaryTraversal{
//       template <class BLOCK_FILTER,class BLOCK>
//       static inline void apply(const BLOCK & block,int i, int j, BLOCK_FILTER & filter){

// 	const int level=BLOCK::level;
// 	typedef typename BLOCK::RealType RealType;

// 	typedef typename Legolas::MultiLevelSparseMatrixInterface<RealType,level>::Matrix::MatrixElementInterface::Copy ElementType;
// 	ElementType aij;
// 	initializeFloatElementToZero(aij);

// 	typename LMatrixInterface::ConstView l=block.L();
// 	typename RMatrixInterface::ConstView r=block.R();

// 	bool aijIsNull=true;

// 	for (int k=0 ; k< l.ncols() ; k++){
// 	  if (!(zeroElement(i,k,l))&&(!zeroElement(k,j,r))){
// 	    //	    INFOS("i="<<i<<" k="<<k<<" lik="<<l.sparseGetElement(i,k));
// 	    //	    INFOS("k="<<k<<" j="<<j<<" rkj="<<r.sparseGetElement(k,j));
// 	    Legolas::plusEqualProduct(l.sparseGetElement(i,k),r.sparseGetElement(k,j),aij);
// 	    aijIsNull=false;
// 	  }
// 	}
	
// 	if (!aijIsNull){
// 	  //	  INFOS("i="<<i<<" j="<<j<<" aij="<<aij);
// 	  typedef typename TRAVERSAL_POLICY:: template Engine<BLOCK_FILTER> ET;
// 	  ET::applyBlock(aij,filter);	
// 	}
//       }
//     };
	    
  private:
    
    typename LMatrixInterface::Matrix L_;
    typename RMatrixInterface::Matrix R_;
  };

}

#endif
