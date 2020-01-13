#ifndef __LEGOLAS_SINGLEELEMENTMATRIXCONTAINER_HXX__
#define __LEGOLAS_SINGLEELEMENTMATRIXCONTAINER_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/SingleElement/SingleElementMatrixData.hxx"

namespace Legolas{

  class SingleElementMatrixContainer
  {
  public:

    template < class MATRIX_OPTIONS, class ELEMENT_DATA_DRIVER>
    class Engine : public MatrixBasicEngine<ELEMENT_DATA_DRIVER> 
    {
    public:

      typedef typename ELEMENT_DATA_DRIVER::Data                       ElementData;
      typedef typename ELEMENT_DATA_DRIVER::RealType                   RealType;

      typedef SingleElementMatrixData<ELEMENT_DATA_DRIVER>            Data;

      static inline 
      ElementData & getSingleElement(int i, int j, Data & A) {
	A.rowIndex()=i;
	A.colIndex()=j;
	return A.value();
      } 
    
      static inline 
      const ElementData & getSingleElement(int i, int j, const Data & A) {
#ifdef GLASS_BOUND_CHECK
	if (i!=A.rowIndex() || j!=A.colIndex()){
	  INFOS("Index out of range");
	  INFOS("i="<<i);
	  INFOS("j="<<j);
	  INFOS("colIndex="<<A.colIndex());
	  INFOS("rowIndex="<<A.rowIndex());
	  throw std::runtime_error("Index out of range");
	}
#endif
	return A.value();
      } 
    
      static inline void resize(int nrows, int ncols, Data & A){
	A.nrows()=nrows;
	A.ncols()=ncols;
      }

    };

  };

}

#endif

  
