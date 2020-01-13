/**
 * project  DESCARTES
 *
 * @file     IterationControl.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ITERATIONCONTROL_HXX__
#define __LEGOLAS_ITERATIONCONTROL_HXX__

#include "UTILITES.hxx"

namespace Legolas{

  template <class VECTOR>
  class IterationControl 
  {

  public :
  
    typedef typename VECTOR::RealType RealType;

    //  IterationControl( int maxIteration, typename VECTOR::value_type epsilon):maxIteration_(maxIteration),
    IterationControl( int maxIteration, RealType epsilon):maxIteration_(maxIteration),
							  epsilon_(epsilon),
							  currentIteration_(0)
    {
      MESSAGE("IterationControl Ctor : maxIteration="<<maxIteration<<" ; epsilon="<<epsilon);
    };

    bool end ( const VECTOR & Residual){

      bool finish=false;

      currentIteration_++;
      if (currentIteration_>maxIteration_){
	finish=true;
	INFOS("currentIteration_>maxIteration_="<<maxIteration_);
      }
      else{
	RealType s=dot(Residual,Residual);
      
	if (s<(epsilon_*epsilon_)){
	  finish=true;
	  std::cout << currentIteration_ << std::endl;
	}
      }
      return finish;
    } 

    // *AP* 16/07/03
    bool end ( const VECTOR X, const VECTOR & Residual){

      bool finish=false;

      currentIteration_++;
      if (currentIteration_>maxIteration_){
	finish=true;
	// INFOS("currentIteration_>maxIteration_");
      }
      else{
	RealType s=dot(Residual,Residual)/dot(X,X);
      
	if (s<(epsilon_*epsilon_)){
	  finish=true;
	}
      }
      return finish;
    } 

    // *AP* 16/07/03
    int getCurrentIteration ( void ) const {

      return currentIteration_ ;
    }
    
  private :
  
    int maxIteration_;
    RealType epsilon_;
    int currentIteration_;


  } ;

}

#endif		/* #if ! defined( __ITERATIONCONTROL__HXX__ ) */
