/**
 * project  DESCARTES
 *
 * @file     NoIterativeLinearSystemAcceleration.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_NOITERATIVELINEARSYSTEMACCELERATION_HXX__
#define __LEGOLAS_NOITERATIVELINEARSYSTEMACCELERATION_HXX__

#include "Debug.hxx"

namespace Legolas{

  // FIXME : dont use it into header files !
  using namespace std ;

  template <class MATRIX_INVERSIBLE, 
	    class MATRIX,            
	    class VECTOR>
  class NoIterativeLinearSystemAcceleration 
  {

  public :

    typedef MATRIX_INVERSIBLE            MatrixInversible;
    typedef MATRIX                       Matrix;
    typedef VECTOR                       Vector;
    typedef typename VECTOR::RealType    RealType;

    NoIterativeLinearSystemAcceleration ( const MATRIX_INVERSIBLE & A,
				 const MATRIX &                       B,
				 const RealType &    lambda,
				 const VECTOR & X,
				 const RealType & fluxExternalPrecision,
				 const DK::Number & verbose): 
      epsf_( fluxExternalPrecision ),
      verbose_( verbose ),
      erreurFlux_(1.F),
      iext_(0),
      fin_(false)
    {
    }
		
    NoIterativeLinearSystemAcceleration ( const MATRIX_INVERSIBLE & A,
				 const MATRIX &                       B,
				 const RealType &    lambda,
				 const VECTOR & X);

    NoIterativeLinearSystemAcceleration( const NoIterativeLinearSystemAcceleration &acceleration );
		
    ~NoIterativeLinearSystemAcceleration( void );

    bool fin ( void ) const
    {
      return fin_;
    }

    void printConvergence( ) const;

    template <class VECTOR_OLD>
    void apply( VECTOR & X , VECTOR_OLD & Xp_)
    {
      iext_ ++;
      // 
      VECTOR    temp(X);
      temp -= Xp_;
      RealType  dotXX =  dot(X,X);
      erreurFlux_ = (RealType) sqrt( dot(temp,temp) / dotXX );
      //
				
      if ( verbose_ >= 2 ) {
	printConvergence( );
      }
      //
      // Test d'arret
      //
      if (erreurFlux_ <= epsf_) 
	{
	  fin_ = true;
	}
      return ;
    }

  private :
    RealType    epsf_;
    DK::Number  verbose_;
    RealType    erreurFlux_;
    int         iext_; // Numero d'iteration externe
    bool        fin_;
  };
  // Initialisation des attributs d'un NoAcceleration
  template<class                       MATRIX_INVERSIBLE,
	   class                                MATRIX,
	   class                                VECTOR>
  NoIterativeLinearSystemAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::NoIterativeLinearSystemAcceleration ( const MATRIX_INVERSIBLE & A,
											    const MATRIX &                       B,
											    const typename VECTOR::RealType &    lambda,
											    const VECTOR & X ) : erreurFlux_(1.F), 
														 iext_(0),
														 fin_(false)
  {
  }

  template<class MATRIX_INVERSIBLE,
	   class MATRIX,          
	   class VECTOR>
  NoIterativeLinearSystemAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::NoIterativeLinearSystemAcceleration(const NoIterativeLinearSystemAcceleration & a): erreurFlux_(a.erreurFlux_), 
																 iext_(a.iext_), 
																 fin_(a.fin_)
  {
  }

  template<class MATRIX_INVERSIBLE,
	   class MATRIX,          
	   class VECTOR>
  NoIterativeLinearSystemAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::~NoIterativeLinearSystemAcceleration()
  {
  }

  template<class MATRIX_INVERSIBLE,
	   class MATRIX,          
	   class VECTOR>
  void NoIterativeLinearSystemAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::printConvergence( ) const
  {
    if( DK::Debug::GetLevel() > 0 )
      {
	std::cout << "IT = " << iext_ << " ERFLUX = " << erreurFlux_ << std::endl;
      }
  }

}

#endif	
