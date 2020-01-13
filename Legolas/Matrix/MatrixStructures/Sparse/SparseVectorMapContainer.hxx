#ifndef __LEGOLAS_SPARSEVECTORMAPCONTAINER_HXX__
#define __LEGOLAS_SPARSEVECTORMAPCONTAINER_HXX__

#include "Legolas/Matrix/MatrixStructures/Helper/AllFilled.hxx"
#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"
#include <vector>
#include <map>
#include <algorithm> /* pour find */

namespace Legolas{

  class SparseVectorMapContainer
  {
  public:



    template < class MATRIX_OPTIONS, class ELEMENT_DATA_DRIVER>
    class Engine : public MatrixBasicEngine<ELEMENT_DATA_DRIVER> 
    {
    public:

      typedef MatrixBasicEngine<ELEMENT_DATA_DRIVER>  MBE;
      typedef typename ELEMENT_DATA_DRIVER::Data      ElementData;
      typedef typename ELEMENT_DATA_DRIVER::RealType  RealType;
    
      typedef std::map<int,ElementData>               Row;
      typedef std::vector< Row >                      Matrix;

      static bool theSparsityPatternIsConstant( void ) { return false;}
    
      class Data : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
      public:

	Data( void ):matrix_(){
	  MESSAGE("SparseVectorMapContainer::Data Default Ctor");
	}
      
	Data & operator = (const Data & source){
	  this->MatrixBasicData<ELEMENT_DATA_DRIVER>::operator =(source);
	  matrix_=source.matrix_;
	  return *this;
	}
      public:

	Row & operator[]( int rowIndex ){ return matrix_[rowIndex]; }
	const Row & operator[]( int rowIndex ) const { return matrix_[rowIndex]; }

	int size( void ) const {
	  return matrix_.size();
	}

	void resize(int nrows){ 
	  matrix_.resize(nrows);
	}

// 	void display( void ) const{
// 	  for (int i=0 ; i < matrix_.size() ; i++){
// 	    for (typename Row::const_iterator iter=matrix_[i].begin() ; iter!=matrix_[i].end() ; ++iter){
// 	      INFOS("matrix["<<i<<"]["<<(*iter).first<<"]="<<(*iter).second);
// 	    }
// 	  }
// 	}

	~Data( void ){
	}	
	
      
      private:
	Matrix matrix_;
      };
      
      
    
      //     static inline int ncols(const Data & data){ 
      //       return MBE::ncols(data); 
      //     }

      static inline 
      ElementData & sparseGetElement(int i, int j, Data & A) {
// 	if (Data::level==2){
// 	  INFOS("Creation de l'element i="<<i<<" j="<<j);
//  	} 
//	INFOS("i="<<i<<" A.size()="<<A.size());
	return A[i][j];
      } 
    
      static inline 
      const ElementData & sparseGetElement(int i, int j, const Data & A) {
	const Row & rowI=A[i];
      
	typename Row::const_iterator it=rowI.find(j);      
      
#ifdef GLASS_BOUND_CHECK
	if (it==rowI.end()){
	  INFOS("Element does not exist");
	  INFOS("i="<<i);
	  INFOS("j="<<j);
	  throw std::runtime_error("Element does not exist");
	}
#endif
	return (*it).second;
      } 
    
    
      class FilledRows: public AllFilled {
      public:
      
	template <class MATRIX>
	FilledRows( const MATRIX & matrix ):AllFilled(MBE::nrows(matrix)),
					    matrix_(matrix)
	{
	  MESSAGE("SparseVectorMap::FilledRows Ctor");
	}
      
      
	class FilledCols{
	public:
	  FilledCols(const Row & rowI):rowI_(rowI)//,iter_(rowI.begin())
	  {
	    MESSAGE("SparseVectorMap::FilledRows::FilledCols Ctor");
	  }
	
	  inline int begin( void ) const {
	    int result=0;
	    if (rowI_.begin()!=rowI_.end()) result=(*(rowI_.begin())).first;
	    //	    INFOS("begin="<<result);
	    return result;
	  }
	
	  inline int end( void ) const {
	    int result=0;
	    if (rowI_.begin()!=rowI_.end()){
	      //	    INFOS("LLLLL");
	      result=(*(--rowI_.end())).first+1;
	    }
	    //	    INFOS("end="<<result);
	    return result;
	  }

	  inline void next( int & j ) const{
	    typename Row::const_iterator it=rowI_.find(j);
	    if (it!=rowI_.end()) it++;
	  
	    if (it!=rowI_.end()){
	      j=(*it).first;	  
	    }
	    else{
	      j=j+1;
	    }
	    return;
	  }

	  // 	inline void next( int & j ){
	  // #ifdef GLASS_BOUND_CHECK
	  // 	  if (j!=(*iter_).first){
	  // 	    INFOS("Sparse next semantic violation");
	  // 	    throw std::runtime_error("Sparse next semantic violation");
	  // 	  }
	  // #endif	
	  // 	  iter_++ ;
	  // 	  j=(*iter_).first;
	  // 	}
	
	private:
	  const Row & rowI_;
	  //	  typename Row::const_iterator iter_;
	};
      
	FilledCols filledColsInRow( int rowIndex ){
	  return FilledCols(matrix_[rowIndex]);
	}


      
      private:
	const Data & matrix_;
      };

      
      /***********************************************************/
      class FilledCols: public AllFilled {
      private:
	
	//Non zero indexes are stored in a vector of list of int.
	//Each vector element indexes_[j] refers to col j
	// and contains a list {i0,..in} of non zero elements {i0,j} in that row
	typedef std::list<int> Col;
	typedef std::vector<Col> AllCols;
	
	AllCols allCols_;
	
	
      public:
	template <class MATRIX>
	FilledCols( const MATRIX & matrix ):AllFilled(MBE::ncols(matrix)),
					    allCols_(MBE::ncols(matrix))
	{
	  
	  for (int i=0; i< matrix.size() ; i++){
	    
	    typename Row::const_iterator it=matrix[i].begin();
	    for (;it!=matrix[i].end();it++){
	      int j=it->first;
	      allCols_[j].push_back(i);
	    }
	    
	  }
	}
	
	class FilledRows{
	private:
	  const Col & col_;
	  int begin_;
	  int end_;
	public:
	  
	  FilledRows(const Col & col):col_(col),
				      begin_(0),
				      end_(0)
	  {
	    if (col_.begin()!=col_.end()){
	      begin_=*(col_.begin());
	      int lastIndex=*(col_.rbegin());
	      //L.P. past the end semantic
	      end_=lastIndex+1;
	    }
	  }
	  
	  inline int begin( void ) const { return begin_;}
	  inline int end( void ) const { return end_;}

	  inline void next( int & j ) const{
	    typename Col::const_iterator jPtr=std::find(col_.begin(),col_.end(),j);
	    j=end_;
	    if (jPtr!=col_.end()){
	      jPtr++;
	      if (jPtr!=col_.end()){
		j=*jPtr;
	      }
	    }
	  }
	  
	};
	
	FilledRows filledRowsInCol( int colIndex ){
	  return FilledRows(allCols_[colIndex]);
	}

      };



    
      template <int LEVEL>
      static inline void resize(const MatrixShape<LEVEL> & shape, Data & A){
	A.setColShape()=shape.getColShape();
	A.setRowShape()=shape.getRowShape();
	A.resize(shape.nrows());
      }

    };

  };

}

#endif

  
