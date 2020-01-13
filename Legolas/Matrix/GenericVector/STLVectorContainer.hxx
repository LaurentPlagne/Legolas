/**
 * project  DESCARTES
 *
 * @file     STLVectorContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_STLVECTORCONTAINER_HXX__
#define __LEGOLAS_STLVECTORCONTAINER_HXX__

#include "UTILITES.hxx"
#include <vector>
#include "Legolas/Matrix/Helper/ConstGetElement.hxx"

namespace Legolas{

  class STLVectorContainer{

  public:
  
    template<class T>
    class Engine{
    public:

      typedef T VectorElement ;
      typedef std::vector<T> Vector;
      typedef typename Vector::size_type size_type;

      static const int padding=0;
    
      static inline 
      size_type size(const Vector & X ) {
	return X.size()-padding;
      } 
    
      static inline 
      typename ConstGetElement<VectorElement>::Type getElement(size_type i, const Vector & X) {
	return X[i];
      } 
    
      static inline 
      VectorElement & getElement(size_type i , Vector & X) {
	return X[i];
      } 
    
      static inline void resize(size_type size, Vector & X){
	if (X.size()-padding!=size_t(size)){
	  X.resize(size+padding);
	}
      }
      static inline void resize(int size, Vector & X){
	if (X.size()-padding!=size_t(size)){
	  X.resize(size+padding);
	}
      }
    };
    
  };



}
#endif
