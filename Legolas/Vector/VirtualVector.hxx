#pragma once

#include <cstdlib>

namespace Legolas{
  class VirtualVector {
  public:
    //    virtual int getSize( void ) const  = 0 ;
    typedef std::size_t SizeType;
    virtual SizeType size( void ) const  = 0 ;
    virtual const VirtualVector & getElement(  SizeType i ) const = 0 ;
    virtual VirtualVector & getElement( SizeType i ) = 0 ;
    virtual VirtualVector * clone( void ) const = 0 ;
    virtual VirtualVector * newVirtualVector( const VirtualVectorShape & shape) const = 0 ;
    virtual ~VirtualVector( void ){};

    virtual bool sameShape(const VirtualVector & source ) const = 0;
    virtual void copy(const VirtualVector & source) = 0 ;
    virtual void scale( double a ) =0 ;
    virtual double relativeDiffAndCopy( const VirtualVector & source ) = 0;
    virtual void plusAssign(double factor, const VirtualVector & source) = 0;
    virtual double dot(const VirtualVector & source) const = 0;
    virtual void scaleAndPlusAssign(double scaleFactor, double factor,const VirtualVector & source) = 0;

    virtual void display( void ) const =0;
    
    typedef VirtualVector * VirtualVectorPtr;
    static inline VirtualVector & getClone(VirtualVectorPtr & vecPtr,const VirtualVector & source){
      if (!vecPtr){
	vecPtr=source.clone();
      }
      else{
	if (!vecPtr->sameShape(source)){
	  delete vecPtr;
	  vecPtr=source.clone();
	}
	else{
	  vecPtr->copy(source);
	}
      }
      return (*vecPtr);
    }

    
  };
}
