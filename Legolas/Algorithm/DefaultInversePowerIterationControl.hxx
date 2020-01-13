/**
 * project  DESCARTES
 *
 * @file     DefaultInversePowerIterationControl.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DEFAULTINVERSEPOWERITERATIONCONTROL_HXX__
#define __LEGOLAS_DEFAULTINVERSEPOWERITERATIONCONTROL_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  template <class ACCELERATION_METHOD>
  class DefaultInversePowerIterationControl 
  {

  public :

    typedef ACCELERATION_METHOD                              AccelerationMethod;
    typedef typename AccelerationMethod::MatrixInversible    MatrixInversible;
    typedef typename AccelerationMethod::Matrix              Matrix;
    typedef typename AccelerationMethod::Vector              Vector;
    typedef typename Vector::RealType                        RealType;

    DefaultInversePowerIterationControl ( const MatrixInversible & A,
					  const Matrix &                                           B,
					  const RealType &                                         lambda,
					  const Vector & X,
					  const DK::Number & maxExternalIteration,
					  const DK::Number & maxNonAcceleratedIteration,
					  const DK::Number & maxAcceleratedIteration,
					  const RealType & fluxExternalPrecision,
					  const RealType & eigenvaluePrecision,
					  const DK::Number & verbose):  maxIteration_ ( maxExternalIteration ),
									currentIteration_(0),
									verbose_ (verbose),
									accelerationMethod_(A,B,lambda,X,
											    maxNonAcceleratedIteration,
											    maxAcceleratedIteration,
											    fluxExternalPrecision,
											    eigenvaluePrecision,
											    verbose)
    {
    }

    bool end ( Vector & X, RealType & lambda, Vector & Xold, RealType dotXX )
    {
      X86Timer    chronometre;
      chronometre.start();
      bool finish = false;
      currentIteration_++;

      //BL : Avoid unusefull iteration when nan is detected         
      if(lambda !=lambda){
	INFOS("ERROR : NaN DETECTED ");
        return true;
      }
      //Fin Modif
	

      if ( currentIteration_ >= maxIteration_ )
	{
	  if ( verbose_ >= 1 ) {
	    std::cout<< "WARNING : LE NOMBRE MAXIMAL D'ITERATIONS EXTERNES (BOUCLE DE LA PUISSANCE) EST ATTEINT : " << maxIteration_ << std::endl;
	    accelerationMethod_.printConvergence(lambda) ;
	  }
	  finish = true;


	  
	}
      else
	{
	  X86Timer    chronos;
	  chronos.start();
	  
	  accelerationMethod_.apply(X,lambda,Xold,dotXX) ;
	  
	  chronos.stop();
	  chronos.storeResult("accelerationMethod_.apply(X,lambda,Xold,dotXX)");
	  chronos.start();

	  finish = accelerationMethod_.fin();

	  chronos.stop();
	  chronos.storeResult("accelerationMethod_.fin()");




	  if ( finish && verbose_ >= 1) {
	    std::cout<< "L'ALGORITHME DE LA PUISSANCE A CONVERGE EN " << currentIteration_ << " ITERATIONS" << std::endl;
	  }
	}
      chronometre.stop();
      chronometre.storeResult("DefaultInversePowerIterationControl::end");
      
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
	
    RealType getFluxError( void ) const { return accelerationMethod_.getFluxError() ; }
    

  public :  
    DK::Number maxIteration_;
    DK::Number currentIteration_;
    DK::Number verbose_;
    AccelerationMethod accelerationMethod_;
  } ;

}

#endif	
