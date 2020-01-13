/**
 * project  DESCARTES
 *
 * @file     NoInversePowerAcceleration.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_NOINVERSEPOWERACCELERATION_HXX__
#define __LEGOLAS_NOINVERSEPOWERACCELERATION_HXX__

#include "Debug.hxx"

namespace Legolas{

  // FIXME : dont use it into header files !
  using namespace std ;

  template <class MATRIX_INVERSIBLE, 
	    class MATRIX,            
	    class VECTOR>
  class NoInversePowerAcceleration 
  {

  public :

    typedef MATRIX_INVERSIBLE            MatrixInversible;
    typedef MATRIX                       Matrix;
    typedef VECTOR                       Vector;
    typedef typename VECTOR::RealType    RealType;

    NoInversePowerAcceleration ( const MATRIX_INVERSIBLE & A,
				 const MATRIX &                       B,
				 const RealType &    lambda,
				 const VECTOR & X,
				 const DK::Number & maxNonAcceleratedIteration,
				 const DK::Number & maxAcceleratedIteration,
				 const RealType & fluxExternalPrecision,
				 const RealType & eigenvaluePrecision,
				 const DK::Number & verbose): 
      epsf_( fluxExternalPrecision ),
      epsvp_( eigenvaluePrecision ),
      verbose_( verbose ),
      maxNonAcceleratedIteration_(maxNonAcceleratedIteration),
      maxAcceleratedIteration_(maxAcceleratedIteration),
      vp_(1.F),
      erreurVp_(1.F),
      erreurFlux_(1.F),
      iext_(0),
      fin_(false)
    {
    }
		
/*    NoInversePowerAcceleration ( const MATRIX_INVERSIBLE & A,
				 const MATRIX &                       B,
				 const RealType &    lambda,
				 const VECTOR & X): epsf_( fluxExternalPrecision ),
                                                    epsvp_( eigenvaluePrecision ),
                                                    verbose_( verbose ),
                                                    maxNonAcceleratedIteration_(0),
                                                    maxAcceleratedIteration_(0),             
                                                    vp_(1.F),
					            erreurVp_(1.F), 
						    erreurFlux_(1.F), 
						    iext_(0),
						    fin_(false)
  {
  }

    NoInversePowerAcceleration( const NoInversePowerAcceleration &acceleration ): epsf_( acceleration.fluxExternalPrecision_ ),
                                                                                  epsvp_( acceleration.eigenvaluePrecision_ ),
                                                                                  verbose_( acceleration.verbose_ ),
                                                                                  maxNonAcceleratedIteration_( acceleration.maxNonAcceleratedIteration_ ),
                                                                                  maxAcceleratedIteration_( acceleration.maxAcceleratedIteration_ ),
                                                                                  vp_(acceleration.vp_), 
										  erreurVp_(acceleration.erreurFlux_), 
										  erreurFlux_(acceleration.erreurFlux_), 
										  iext_(acceleration.iext_), 
										  fin_(acceleration.fin_)
  {
  }*/
		
    virtual ~NoInversePowerAcceleration( void )
    {
    }

    bool fin ( void ) const
    {
      return fin_;
    }

    void printConvergence( const RealType & vp ) const
    {
    if( DK::Debug::GetLevel() > 0 )
      {
	std::cout << "IT = " << iext_ << " VP = " << vp << " ERVP = " << erreurVp_ 
		  << " ERS = " << erreurFlux_ << std::endl;
      }
  }

    template <class VECTOR_OLD>
    void apply( VECTOR & X , RealType & vp, VECTOR_OLD & Xp_, RealType & dotXX)
    {
      iext_ ++;
      // 
      erreurVp_ = (RealType) fabs( vp-vp_ ) / vp ;
      VECTOR    temp(X);
      temp -= Xp_;
      erreurFlux_ = (RealType) sqrt( dot(temp,temp) / dotXX );
      //
				
      if ( verbose_ >= 2 ) {
	printConvergence( vp );
      }
      vp_ = vp ;
      //
      // Test d'arret
      //
      if ( (erreurFlux_ <= epsf_) && (erreurVp_ <= epsvp_) ) 
	{
	  fin_ = true;
	}
      return ;
    }

  private :
    RealType    epsf_;
    RealType    epsvp_;
    DK::Number  verbose_;
    DK::Number      maxNonAcceleratedIteration_;
    DK::Number      maxAcceleratedIteration_;
    RealType                             vp_;
    RealType                             erreurVp_;
    RealType                             erreurFlux_;
    int                                  iext_; // Numero d'iteration externe
    bool                                 fin_;
  };

}

#endif	
