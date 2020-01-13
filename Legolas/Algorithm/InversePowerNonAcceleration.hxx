/**
 * project  DESCARTES
 *
 * @file     InversePowerNonAcceleration.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_INVERSEPOWERNONACCELERATION_HXX__
#define __LEGOLAS_INVERSEPOWERNONACCELERATION_HXX__

#include "Debug.hxx"

namespace Legolas{

  // FIXME : dont use it into header files !
  using namespace std ;

  template <class MATRIX_INVERSIBLE, 
	    class MATRIX,            
	    class VECTOR>
  class InversePowerNonAcceleration 
  {

  public :

    typedef MATRIX_INVERSIBLE            MatrixInversible;
    typedef MATRIX                       Matrix;
    typedef VECTOR                       Vector;
    typedef typename VECTOR::RealType    RealType;

    InversePowerNonAcceleration ( const MATRIX_INVERSIBLE & A,
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
      verbose_( verbose )
    {
    }
		
    InversePowerNonAcceleration ( const MATRIX_INVERSIBLE & A,
				  const MATRIX &                       B,
				  const RealType &    lambda,
				  const VECTOR & X);

    InversePowerNonAcceleration( const InversePowerNonAcceleration &acceleration );
		
    ~InversePowerNonAcceleration( void );

    bool fin ( void ) const
    {
      return fin_;
    }

    void printConvergence( const RealType & vp ) const;

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
				
      if ( verbose_ == 2 ) {
	printConvergence( vp );
      }
      vp_ = vp ;
      //
      // Test d'arret
      //
      if ( (erreurFlux_ <= epsf_) && (erreurVp_ <= epsvp_) ) 
	{
	  //
	  // La décroissance du résidu n'est pas 
	  // monotone. Il se peut donc qu'on vérifie le critère
	  // d'arrêt à une itération et qu'on ne le respecte
	  // pas à l'itération suivante. Supposons qu'on
	  // ait effectué un premier calcul convergé et qu'on 
	  // sorte à l'issue de ce calcul avec X. Si 
	  // on effectue une seconde fois le même calcul, 
	  // on pourrait effectuer plus d'une itération car 
	  // le résidu peut remonter au dessus du critère
	  // Pour empêcher cela, on sort avec Xold et non X.
	  //
	  X = Xp_;
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
  // Initialisation des attributs d'un NoAcceleration
  template<class                       MATRIX_INVERSIBLE,
	   class                                MATRIX,
	   class                                VECTOR>
  InversePowerNonAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::InversePowerNonAcceleration ( const MATRIX_INVERSIBLE & A,
											      const MATRIX &                       B,
											      const typename VECTOR::RealType &    lambda,
											      const VECTOR & X ) : vp_(1.F),
														   erreurVp_(1.F), 
														   erreurFlux_(1.F), 
														   iext_(0),
														   fin_(false)
  {
  }

  template<class MATRIX_INVERSIBLE,
	   class MATRIX,          
	   class VECTOR>
  InversePowerNonAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::InversePowerNonAcceleration(const InversePowerNonAcceleration & a): vp_(a.vp_), 
																    erreurVp_(a.erreurFlux_), 
																    erreurFlux_(a.erreurFlux_), 
																    iext_(a.iext_), 
																    fin_(a.fin_)
  {
  }

  template<class MATRIX_INVERSIBLE,
	   class MATRIX,          
	   class VECTOR>
  InversePowerNonAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::~InversePowerNonAcceleration()
  {
  }

  template<class MATRIX_INVERSIBLE,
	   class MATRIX,          
	   class VECTOR>
  void InversePowerNonAcceleration<MATRIX_INVERSIBLE,MATRIX,VECTOR>::printConvergence( const RealType & vp ) const
  {
    if( DK::Debug::GetLevel() > 0 )
      {
	std::cout << "IT = " << iext_ << " VP = " << vp << " ERVP = " << erreurVp_ 
		  << " ERS = " << erreurFlux_ << std::endl;
      }
  }

}

#endif	
