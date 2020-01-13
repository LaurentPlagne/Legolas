#ifndef __LEGOLAS_SPARSESCALARVECMAPCONTAINER_HXX__
#define __LEGOLAS_SPARSESCALARVECMAPCONTAINER_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  class SparseScalarVecMapContainer{
  public:
    template <class SCALAR>
    class Engine: public Matrix{
    public:
      typedef SCALAR RealType;
      typedef Engine<SCALAR> Container;
    private:
      typedef std::map<int,SCALAR>  Row;
      typedef std::vector< Row >    SparseData;

      SparseData sparseData_;

    
    public:
      
      virtual ~Engine( void ){}


      inline Engine(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape),
								   sparseData_(virtualMatrixShape.nrows()){}

      inline Engine(const Container & container):Matrix(container),
						 sparseData_(container.sparseData_){}
      
      
      
      inline void setSparseElement(int i, int j, const SCALAR & value){
	sparseData_[i][j]=value;
      }
      
      inline const SCALAR & sparseGetElement(int i, int j) const{
	const Row & rowI=sparseData_[i];
	
	typename Row::const_iterator it=rowI.find(j);      
	
	if (it==rowI.end()){
	  INFOS("Element does not exist");
	  INFOS("i="<<i);
	  INFOS("j="<<j);
	  throw std::runtime_error("Element does not exist");
	}
	return (*it).second;
      }
      
      inline SCALAR & sparseGetElement(int i, int j){ 
	Row & rowI=sparseData_[i];
	typename Row::iterator it=rowI.find(j);      
	
	if (it==rowI.end()){
	  INFOS("Element does not exist");
	  INFOS("i="<<i);
	  INFOS("j="<<j);
	  throw std::runtime_error("Element does not exist");
	}
	return (*it).second;
      }
      
      inline int beginRow( void ) const {return 0;}
      inline int endRow( void ) const {return sparseData_.size(); }
      
      inline int beginColInRow( int i ) const {
	int result=0;
	const Row & rowI=sparseData_[i];
	if (rowI.begin()!=sparseData_[i].end()) result=(*(rowI.begin())).first;
	return result;;
      }
      inline int endColInRow( int i ) const {
	int result=0;
	const Row & rowI=sparseData_[i];
	if (rowI.begin()!=rowI.end()){
	  result=(*(--rowI.end())).first+1;
	}
	return result;
      }
      
      inline int nextColInRow(int i, int j) const {
	const Row & rowI=sparseData_[i];
	typename Row::const_iterator it=rowI.find(j);
	if (it!=rowI.end()) it++;
      
	int result=j+1;
	
	if (it!=rowI.end()){
	  result=(*it).first;	  
	}
	
	return result;
      }
      
    };

  };

}

#endif
    
    
    
    
