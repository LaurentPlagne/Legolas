/**
 * project  DESCARTES
 *
 * @file    IterativeLinearSystemTchebychevAcceleration.hxx
 *
 * @aut horMaxime Barrault
 * @date   june 2008
 *
 * This file allows us to accelerate the iterative resolution of the source problem
 * by an optimal tchebychev acceleration process (cf. work of M. Barrault). Instead of
 * the what is done in the file ModifiedMinosTchebychevAcceleration.hxx, the estimation
 * of the higher eigenvalue of the iteration matrix is needed. If this estimation is
 * zero, the algorithm is not accelerated. If this estimation is not in [0,1[, the
 * algorithm is stopped at the beginning as the iterative process can't converge. 
 *
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ITERATIVELINEARSYSTEMTCHEBYCHEVACCELERATION_HXX__
#define __LEGOLAS_ITERATIVELINEARSYSTEMTCHEBYCHEVACCELERATION_HXX__

#include "Debug.hxx"

namespace Legolas{

  template <class MATRIX_INVERSIBLE, 
	    class MATRIX,            
	    class VECTOR>
  class IterativeLinearSystemTchebychevAcceleration 
  {
  public :

    typedef MATRIX_INVERSIBLE            MatrixInversible;
    typedef MATRIX                       Matrix;
    typedef VECTOR                       Vector;
    typedef typename Vector::RealType    RealType;

    IterativeLinearSystemTchebychevAcceleration ( const MATRIX_INVERSIBLE & A,
					  const MATRIX &                       B,
					  const RealType &    lambda,
					  const VECTOR & X,
					  const RealType & fluxExternalPrecision,
					  const DK::Number & verbose): 
      epsf_( fluxExternalPrecision ),
      verbose_( verbose ),
      Xpp_(X),
      erreurFlux_(1.F), 
      sigma_(0.F),
      gamma_(0.F),
      rho_(1.F),
      iext_(0), 
      fin_(false)
    {
	    if ( lambda < 0. && lambda >= 1. ) {
		    std::cout << "WARNING : the value of lambda should be between 0 and 1 for the convergence of the iterative process. So, the iterative process is stopped at the beginning and the found solution is bad" << std::endl;;
		    fin_ = true;
	    } else {
		    gamma_ = 2. / ( 2. - lambda );
		    sigma_ = lambda / ( 2. - lambda );
	    }
    }

    ~IterativeLinearSystemTchebychevAcceleration( void ){};

    template <class VECTOR_OLD>
    void apply( VECTOR & X , VECTOR_OLD & Xp_ )
    {
      iext_ ++ ;

      RealType somme=Legolas::squareNorm2(X-Xp_);
      RealType dotXX = dot(X,X);
      RealType residual = somme / dotXX;
      if(std::isnan(dotXX))
      {
          INFOS("WARNING : dot(X,X) = NAN. SORTIE DE LA BOUCLE ITERATIVE");
          fin_=true;
      }
      
      //	
      erreurFlux_ = (RealType) sqrt( residual );
      //
      // Test d'arret
      //
      if ( erreurFlux_ <= epsf_ || fin_ == true ) 
	{
		fin_ = true;
	} else {
	      if ( sigma_ > 0. ) { //sigma_=0 when lambda = 0 <-> no acceleration
		      this->accelerate( X , Xp_);
	      }
      }
      //
      if ( verbose_ >= 2 ) {
	printConvergence( );
      }

      Xpp_ = Xp_;

      return ;
    }
  
    bool fin ( void ) const
    {
      return fin_;
    }

    void printConvergence( void ) const 
    {
      std::cout<< "IT = " << iext_<< " ERFLUX = " << erreurFlux_ << std::endl ;
    }

  private :
  
    RealType    epsf_;
    DK::Number  verbose_;
    VECTOR             Xpp_;
    RealType           erreurFlux_;
    RealType           romg_;                          // |
    RealType           rsig_;                          // |
    RealType           sigma_;                         // 
    RealType           gamma_;
    RealType           rho_;
    DK::Number         iext_;                          // Numero d'iteration externe
    bool               fin_;

    template <class    VECTOR_OLD>
    void accelerate( VECTOR & X, VECTOR_OLD & Xp_)
	  {
		  if ( iext_ == 1 ) {
			  RealType mu = rho_ * gamma_;
			  X = mu * X + (1 - mu)  * Xp_;
			  //std::cerr << "Tchebitchev : Coefficients = " << mu << " " << 1-mu << std::endl;
		  } else {
			  if ( iext_ == 2 ) {
				  rho_ = 1. / ( 1. - sigma_ * sigma_ / 2. );
			  } else {
				  rho_ = 1. / ( 1. - sigma_ * sigma_ * rho_ / 4. );
			  }
			  RealType mu = rho_ * gamma_;
			  RealType nu = 1. - rho_;
			  X = mu * X + (1 - nu - mu)  * Xp_ + nu * Xpp_;
			  //std::cerr << "Tchebitchev : Coefficients = " << mu << " " << 1-nu-mu << " " << nu << std::endl;
		  }
		  return;
	  }
  };
}

#endif	
