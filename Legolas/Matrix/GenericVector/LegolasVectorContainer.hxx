/**
 * project  DESCARTES
 *
 * @file     LegolasVectorContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_LEGOLASVECTORCONTAINER_HXX__
#define __LEGOLAS_LEGOLASVECTORCONTAINER_HXX__

#include "UTILITES.hxx"
#include <vector>
#include "Legolas/Matrix/Helper/ConstGetElement.hxx"
#include "Legolas/Vector/Vector.hxx"

namespace Legolas{

  class LegolasVectorContainer{

  public:
  
    template<class T>
    class Engine{
    public:

      typedef T VectorElement ;
      typedef Legolas::Vector<T> Vector;
      typedef size_t size_type;

      static inline 
      size_type size(const Vector & X ){
	return X.size();
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
	X.resize(size);
      }
      
      static inline void resize(int size, Vector & X){
	X.resize(size);
      }
    };
    
  };



}
#endif
