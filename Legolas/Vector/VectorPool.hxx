#ifndef __VECTOR_POOL_HXX__
#define __VECTOR_POOL_HXX__

#include <pthread.h>


#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Shape/ShapeTraits.hxx"

#include <map>

namespace Legolas{
  
  //  class AnyVectorPool{};
  
  
  template <class REAL_TYPE, int LEVEL>
  class VectorPool: public VectorPool<REAL_TYPE,LEVEL-1>
  {
  private:    
    typedef typename ShapeTraits<LEVEL>::Type Shape;
    typedef Legolas::MultiVector<REAL_TYPE,LEVEL>      Vector;
    typedef Vector  *         VectorPtr;
    typedef std::multimap<Shape,VectorPtr>    AvailableVectorMap;
    typedef std::map<VectorPtr,Shape>         UsedVectorMap;
    
    AvailableVectorMap availableVectorMap_;
    UsedVectorMap usedVectorMap_;

    pthread_mutex_t mutexsum;
    
  public:
    
    VectorPool( void ):availableVectorMap_(),
		       usedVectorMap_(),
		       mutexsum(){
      pthread_mutex_init(&mutexsum, NULL);
    }
		       
    

    //See google for explanation....
    using VectorPool<REAL_TYPE,LEVEL-1>::aquireVectorPtr;
    using VectorPool<REAL_TYPE,LEVEL-1>::releaseVectorPtr;

    typename Vector::Element * aquireElementPtr(const typename Vector::Element & source){
      return VectorPool<REAL_TYPE,LEVEL-1>::aquireVectorPtr(Legolas::getShape(source));
    }

    void releaseElementPtr(typename Vector::Element * elementPtr){
      VectorPool<REAL_TYPE,LEVEL-1>::releaseVectorPtr(elementPtr);
    }

    MultiVector<REAL_TYPE,LEVEL> * aquireVectorPtr(const typename ShapeTraits<LEVEL>::Type & shape){
      
      pthread_mutex_lock (&mutexsum);
      typename AvailableVectorMap::iterator iter=availableVectorMap_.find(shape);

      MultiVector<REAL_TYPE,LEVEL> * result=0;

      if (iter==availableVectorMap_.end()){// no vector with this shape is available
	result=new Vector(shape);
	//	INFOS("Vector Ctor ("<<result<<") with shape = "<<shape<<" ("<<result->sizeInMB()<<" MB)");
      }
      else{
	//	INFOS("(*iter).first="<<(*iter).first);
	//	INFOS("shape="<<shape);

	assert(((*iter).first==shape));
	  
	result=(*iter).second; // aquire vector ptr;
	availableVectorMap_.erase(iter); //remove item from tha available map;
	//	INFOS("Vector available ("<<result<<") with shape = "<<shape);
      }
      
      usedVectorMap_.insert(std::make_pair(result,shape));

      pthread_mutex_unlock (&mutexsum);
      return result;
    }
    
    void releaseVectorPtr(MultiVector<REAL_TYPE,LEVEL> * vectorPtr){
      
      typename UsedVectorMap::iterator iter=usedVectorMap_.find(vectorPtr);
      availableVectorMap_.insert(std::make_pair((*iter).second,vectorPtr));//Create item in the available map
      usedVectorMap_.erase(iter); //remove item from the used vector map
      
      return;
    }

    ~VectorPool( void ){

      if (!(usedVectorMap_.empty())){
	INFOS("A vector has not been properly released !!");
	BACKTRACE;
	throw std::runtime_error("A vector has not been properly released !!");
      }
      
      typename AvailableVectorMap::iterator iter=availableVectorMap_.begin();
      double sum=0.0;

      for ( ; iter!=availableVectorMap_.end(); iter++){
	assert((*iter).second!=0);
	sum+=(*iter).second->sizeInMB();
	delete (*iter).second;
      }
      //      INFOS("Total Memory Releasead By this Memory Pool :"<<sum<<" MB");
      

      pthread_mutex_destroy(&mutexsum);
    }
  };

  template <class REAL_TYPE>
  class VectorPool<REAL_TYPE,0>{
  public:
    REAL_TYPE * aquireVectorPtr(const int & size){
      INFOS("ICI");
      REAL_TYPE * result=new REAL_TYPE;
      return result;
    }
    
    void releaseVectorPtr(REAL_TYPE * valuePtr){
      delete valuePtr;
    }


  };

}

#endif
    
  
