/**
 * project  DESCARTES
 *
 * @file     NoLinearSystemAcceleration.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_NOLINEARSYSTEMACCELERATION_HXX__
#define __LEGOLAS_NOLINEARSYSTEMACCELERATION_HXX__

namespace Legolas{

  using namespace std ;

  template <class MATRIX, class VECTOR, class VECTOR_INOUT>
  class NoLinearSystemAcceleration 
  {

  public :

    typedef MATRIX Matrix ;
    typedef VECTOR Vector ;
    typedef VECTOR_INOUT VectorInOut ;
    typedef typename VECTOR_INOUT::RealType RealType ;

    NoLinearSystemAcceleration( const MATRIX & A, 
				const VECTOR & B, 
				const VECTOR_INOUT & X ) ;
    NoLinearSystemAcceleration( const NoLinearSystemAcceleration &acceleration ) ; 
    ~NoLinearSystemAcceleration( void ) ;
    void apply( const VECTOR_INOUT & X ) ;
    bool end ( void ) const ;

  private :

    bool finish_ ;

  } ;


  template<class MATRIX, class VECTOR, class VECTOR_INOUT>
  NoLinearSystemAcceleration<MATRIX,VECTOR,VECTOR_INOUT>::NoLinearSystemAcceleration( const MATRIX & A, 
										      const VECTOR & B, 
										      const VECTOR_INOUT & X ) : finish_(false) 
  {
  }

  template<class MATRIX, class VECTOR, class VECTOR_INOUT>
  NoLinearSystemAcceleration<MATRIX,VECTOR,VECTOR_INOUT>::NoLinearSystemAcceleration(const NoLinearSystemAcceleration & a) : finish_(a.finish_)
  {
  }

  template<class MATRIX, class VECTOR, class VECTOR_INOUT>
  NoLinearSystemAcceleration<MATRIX,VECTOR,VECTOR_INOUT>::~NoLinearSystemAcceleration( void )
  {
  }

  template<class MATRIX, class VECTOR, class VECTOR_INOUT>
  void NoLinearSystemAcceleration<MATRIX,VECTOR,VECTOR_INOUT>::apply( const VECTOR_INOUT & X )
  {
    return ;
  }

}

#endif	
