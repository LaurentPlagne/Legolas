/**
 * project  DESCARTES
 *
 * @file     DefaultLinearSystemIterationControl.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DEFAULTLINEARSYSTEMITERATIONCONTROL_HXX__
#define __LEGOLAS_DEFAULTLINEARSYSTEMITERATIONCONTROL_HXX__

#include "UTILITES.hxx"

namespace Legolas{


  template <class ACCELERATION_METHOD>
  class DefaultLinearSystemIterationControl 
  {

  public :
  
    typedef ACCELERATION_METHOD AccelerationMethod;
    typedef typename AccelerationMethod::Matrix Matrix;
    typedef typename AccelerationMethod::Vector Vector;
    typedef typename AccelerationMethod::VectorInOut VectorInOut;
    typedef typename Vector::RealType RealType;

    DefaultLinearSystemIterationControl( const Matrix & A, 
					 const Vector & B, 
					 const VectorInOut & X) : currentIteration_(0),
								  Xold_(),
								  accelerationMethod_(A,B,X),
								  maxIteration_(A.getMaxIteration()),
								  epsilon_(A.getEpsilon())								
    {
      if (maxIteration_>1){
	Xold_=X;
      }
      
      //      INFOS("maxIteration="<<maxIteration_);
      //      INFOS("X.shape()="<<X.shape());
      //      INFOS("Xold_.shape()="<<Xold_.shape());
    };

    int & maxIteration( void ){ return maxIteration_ ; }
    const int & maxIteration( void ) const { return maxIteration_ ; }

    RealType & epsilon( void ){ return epsilon_ ; }
    const RealType & epsilon( void ) const { return epsilon_ ; }
    
    bool end ( VectorInOut & X ){

      bool finish=false;

      currentIteration_++;

      if (currentIteration_>=maxIteration_){
	//	INFOS("ICI");
	finish=true;
      }
      else {
	accelerationMethod_.apply(X) ;
	//	INFOS("Xold_="<<Xold_);
	Xold_ -= X ;
	//	INFOS("Xold_="<<Xold_);
	RealType s=dot(Xold_,Xold_)/dot(X,X);

	//	INFOS("s="<<s);

	if (s<(epsilon_*epsilon_)){
	  finish=true;
	}
	Xold_ = X ;
      }
      return finish;
    } 

    int getCurrentIteration ( void ) const {

      return currentIteration_ ;
    }
    
  private :

    int currentIteration_;
    VectorInOut Xold_ ;

    ACCELERATION_METHOD accelerationMethod_ ;

  public:
    int maxIteration_;
    RealType epsilon_;


  } ;
}

#endif	
