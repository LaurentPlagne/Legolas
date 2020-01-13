/**
 * project  DESCARTES
 *
 * @file     DefaultIterativeLinearSystemControl.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DEFAULTITERATIVELINEARSYSTEMCONTROL_HXX__
#define __LEGOLAS_DEFAULTITERATIVELINEARSYSTEMCONTROL_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  template <class ACCELERATION_METHOD>
  class DefaultIterativeLinearSystemControl 
  {

  public :

    typedef ACCELERATION_METHOD                              AccelerationMethod;
    typedef typename AccelerationMethod::MatrixInversible    MatrixInversible;
    typedef typename AccelerationMethod::Matrix              Matrix;
    typedef typename AccelerationMethod::Vector              Vector;
    typedef typename Vector::RealType                        RealType;

    DefaultIterativeLinearSystemControl ( const MatrixInversible & A,
					  const Matrix &                                           B,
					  const RealType &                                         lambda,
					  const Vector & X,
					  const DK::Number & maxExternalIteration,
					  const RealType & fluxExternalPrecision,
					  const DK::Number & verbose):  maxIteration_ ( maxExternalIteration ),
									currentIteration_(0),
									verbose_ (verbose),
									accelerationMethod_(A,B,lambda,X,
											    fluxExternalPrecision,
											    verbose)
    {
    }

    bool end ( Vector & X, Vector & Xold )
    {

      bool finish = false;
      currentIteration_++;

      accelerationMethod_.apply(X,Xold);

      if ( currentIteration_ >= maxIteration_ ) {
	      if ( verbose_ >= 1 ) {
		      std::cout<< "WARNING : LE NOMBRE MAXIMAL D'ITERATIONS EXTERNES (BOUCLE DU PROBLEME A SOURCE) EST ATTEINT : " << maxIteration_ << std::endl;
	      }
	      finish = true;
      } else {
	      finish = accelerationMethod_.fin();
      }
     
      if ( finish && verbose_ >= 1) {
	      std::cout<< "L'ALGORITHME D'INVERSION DU PROBLEME A SOURCE A CONVERGE EN " << currentIteration_ << " ITERATIONS" << std::endl;
      }
      //
      return finish;
    } 

    DK::Number getCurrentIteration ( void ) const {
      return currentIteration_;
    }

    DK::Number getMaxExternalIteration ( void ) const {
      return maxIteration_;
    }
	
    DK::Number getVerbose ( void ) const {
      return verbose_;
    }
	

  public :  
    DK::Number maxIteration_;
    DK::Number currentIteration_;
    DK::Number verbose_;
    AccelerationMethod accelerationMethod_;
  } ;

}

#endif	
