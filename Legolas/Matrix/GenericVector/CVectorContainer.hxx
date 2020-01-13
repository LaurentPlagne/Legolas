/**
 * project  DESCARTES
 *
 * @file     CVectorContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_CVECTORCONTAINER_HXX__
#define __LEGOLAS_CVECTORCONTAINER_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/Helper/ConstGetElement.hxx"
//#include "CVectorInitTraits.hxx"

namespace Legolas{

  class CVectorContainer{

  public:

    template<class T>
    class Engine{
    public:
      typedef T VectorElement ;
    
      class CVector{
      public:
	CVector( void ):size_(0),data_(0)
	{
	  //INFOS("addresse de CVector : "<<&(*this));
	  //	INFOS("CVector default Ctor");
	}
	~CVector( void )
	{
	  if (data_!=0) {
	    delete[] data_;
	    data_=0;
	  }
	}
      
	CVector( const CVector & source ):size_(source.size()),data_(0)
	{
	  //	INFOS("CVector copy Ctor");
	  //	INFOS("addresse de CVector : "<<&(*this));
	  data_=new T[size_];
	  for (int i=0 ; i<size_ ; i++){
	    data_[i]=source[i];
	  }
	}
      
	CVector& operator= ( const CVector & source )
	{
	  if( &source != this )
	    {
	      if (source.size()!=size_) this->resize(source.size());	      
	    
	      for( int i=0 ; i<size_ ; i++ )
		{
		  data_[i]=source[i];
		  //  *(_coor+k) = *(vecteur._coor+k) ;
		}
	    }
	  return *this ;
	}
      
      
      
	inline int & size( void ) { return size_ ;}
	inline int size( void ) const {
	  //	INFOS("addresse de CVector : "<<&(*this));
	  //	INFOS("size_="<<size_);
	  return size_;
	}
      
	inline T * getPointer( void ){return data_ ;}
	inline const T * getPointer( void ) const {return data_ ;}
      
	inline T & operator[]( int i ){
	  return data_[i];
	}
      
	inline const T & operator[]( int i ) const {
	  return data_[i];
	}
      
	void resize( int size ){
	  if (size!=size_){
	    if (data_!=0) delete[] data_;
	    data_=new T[size];
	    size_=size;
	  }
	  //	CVectorInitTraits<T>::initialize(data_,size);
	  // LP ?
	  //	for (int i=0 ; i < size ; i++) data_[i]=T();

	}
      
      private:
	int size_ ;
	T * data_ ;
      };
  
  
      typedef CVector Vector;
    
      static inline 
      int size(const Vector & X ) {
	return X.size();
      } 
    
      static inline 
      typename ConstGetElement<VectorElement>::Type getElement(int i, const Vector & X) {
	return X[i];
      } 
    
      static inline 
      VectorElement & getElement(int i, Vector & X) {
	return X[i];
      } 
    
      static inline void resize(int size, Vector & X){
	if (X.size()!=size){
	  X.resize(size);
	}
      }
    
    };
  };

}

#endif
