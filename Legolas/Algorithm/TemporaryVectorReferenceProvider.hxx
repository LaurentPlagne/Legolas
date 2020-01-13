#ifndef __LEGOLAS_TEMPORARYVECTORREFERENCEPROVIDER_HXX__
#define __LEGOLAS_TEMPORARYVECTORREFERENCEPROVIDER_HXX__

#include "Legolas/Vector/VectorPool.hxx"

namespace Legolas{

  template <class REAL_TYPE, int level>
  class TemporaryVectorReferenceProvider{
    typedef REAL_TYPE                    RealType;
    typedef VectorPool<RealType,level> * VectorPoolPtr;
    typedef typename ShapeTraits<level>::Type Shape;
    typedef MultiVector<RealType,level> * VectorPtr;
    
    VectorPoolPtr vectorPoolPtr_;
    std::vector< VectorPtr > vectorPtrArray_;
    
  public:

    typedef MultiVector<RealType,level> & ReturnType;
    
    TemporaryVectorReferenceProvider(const VectorPoolPtr & vectorPoolPtr):vectorPoolPtr_(vectorPoolPtr),
									  vectorPtrArray_()
    {}
    
    
    inline ReturnType getItem(const Shape & shape){
      VectorPtr vectorPtr=vectorPoolPtr_->aquireVectorPtr(shape);
      //VectorPtr vectorPtr=new MultiVector<RealType,level>(shape);
      vectorPtrArray_.push_back(vectorPtr);
      //      (*vectorPtr)=1.0;
      return *vectorPtr;
    }
    
    ~TemporaryVectorReferenceProvider( void ){
      for (uint i=0 ; i<vectorPtrArray_.size() ; i++){
	//	delete vectorPtrArray_[i];
	vectorPoolPtr_->releaseVectorPtr(vectorPtrArray_[i]);
      }
    }
  };

  // Scalar Specialization 
  template <class REAL_TYPE>
  class TemporaryVectorReferenceProvider<REAL_TYPE,0>{
    typedef REAL_TYPE                    RealType;
    typedef VectorPool<RealType,0> * VectorPoolPtr;
    typedef int Shape;
    //    typedef typename ShapeTraits<0>::Type Shape;
    
    
    
  public:

    typedef RealType ReturnType;
    
    TemporaryVectorReferenceProvider(const VectorPoolPtr & vectorPoolPtr){}
    
    inline ReturnType getItem(const Shape & shape){return 0.0;}
    
  };

}

#endif
  

