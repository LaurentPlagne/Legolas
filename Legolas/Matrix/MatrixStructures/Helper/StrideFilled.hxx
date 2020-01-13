/**
 * project  DESCARTES
 *
 * @file     StrideFilled.hxx
 *
 * @author Angelique PONCOT Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_STRIDEFILLED_HXX__
#define __LEGOLAS_STRIDEFILLED_HXX__

namespace Legolas{

  class StrideFilled{
  public:

    StrideFilled( void ):first_(-1),stride_(-1),size_(-1){}

    StrideFilled( int first, int stride, int size ):first_(first),
						    stride_(stride),
						    size_(size)
    {
    }
    inline int begin( void ) const {
      return first_;
    }

    inline int end( void ) const {
      return size_*stride_+first_;
    }
    
    inline void next( int & i ) const {
      i+=stride_;
      return;
    }

    inline int size( void ) const {
      return size_;
    }

    
    inline int & setFirst( void ){ return first_;}
    inline int & setStride( void ){ return stride_;}
    inline int & setSize( void ){ return size_;}


    std::ostream& display(std::ostream& os) const{
      os << "StrideFilled : (first="<<this->first_<<" stride="<<this->stride_<<" size="<<this->size_<<")" ;
      return os;
    }      
    
  private:
    int first_;
    int stride_;
    int size_;
  };
    
  inline std::ostream& operator<< ( std::ostream& os, 
				    const StrideFilled & source )
  {
    return source.display(os);
  }

}

#endif
    
