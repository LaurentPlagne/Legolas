#ifndef __LEGOLAS_MATRIXBASICENGINE_HXX__
#define __LEGOLAS_MATRIXBASICENGINE_HXX__

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"

namespace Legolas{

  template <class ELEMENT_DATA_DRIVER>
  struct MatrixBasicEngine{

    typedef Actual  StorageMode ;   

    typedef typename ELEMENT_DATA_DRIVER::RealType RealType;
    typedef typename ELEMENT_DATA_DRIVER::Data     ElementData;
  
    template <class DATA>
    static inline ElementData & zero(DATA & data){
      return data.zero();
    }
  
    template <class DATA>
    static inline const ElementData & zero(const DATA & data){
      return data.zero();
    }

    template <class DATA>
    static inline int nrows(const DATA & A ) {
      return A.nrows();
    } 
  
    template <class DATA>
    static inline int ncols(const DATA & A ) {
      return A.ncols();
    } 
  
    template <class DATA>
    static inline int getMaxIteration(const DATA & A){
      return A.getMaxIteration();
    }
  
    template <class DATA>
    static inline int & setMaxIteration(DATA & A){
      return A.setMaxIteration();
    }
  
    template <class DATA>
    static inline RealType getEpsilon(const DATA & A){
      return A.getEpsilon();
    }
  
    template <class DATA>
    static inline RealType & setEpsilon(DATA & A){
      return A.setEpsilon();
    }


    template <class DATA>
    static inline typename DATA::Shape & setRowShape(DATA & A){
      return A.setRowShape();
    }

    template <class DATA>
    static inline typename DATA::Shape & setColShape(DATA & A){
      return A.setColShape();
    }


    template <class DATA>
    static inline const typename DATA::Shape & getRowShape(const DATA & A){
      return A.getRowShape();
    }

    template <class DATA>
    static inline const typename DATA::Shape & getColShape(const DATA & A){
      return A.getColShape();
    }

  };

}

#endif

  
