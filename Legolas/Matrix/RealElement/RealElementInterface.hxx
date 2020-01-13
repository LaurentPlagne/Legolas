/**
 * project  DESCARTES
 *
 * @file     RealElementInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_REALELEMENTINTERFACE_HXX__
#define __LEGOLAS_REALELEMENTINTERFACE_HXX__

#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"

namespace Legolas{

  template <class REAL_TYPE>
  class RealElementInterface{
  public:
    typedef RealDataDriver<REAL_TYPE> DataDriver;

    //    static bool isAMatrix( void ) { return false ; }
    static const bool isAMatrix_=false;

    typedef REAL_TYPE           Copy;
    typedef REAL_TYPE &         View; 
    typedef const REAL_TYPE &   ConstView;

    struct Solver{
      Solver( void ){}
      Solver(const REAL_TYPE & aii, REAL_TYPE & xi, const REAL_TYPE & bi){}
      template <class VECTOR_POOL_PTR>
      Solver(const REAL_TYPE & aii, REAL_TYPE & xi, const REAL_TYPE & bi,const VECTOR_POOL_PTR & vptr){}
      
      inline void solve(const REAL_TYPE & aii, REAL_TYPE & xi, const REAL_TYPE & bi){
	xi=bi/aii;
      }

      inline void transposeSolve(const REAL_TYPE & aii, REAL_TYPE & xi, const REAL_TYPE & bi){
	xi=bi/aii;
      }
    };

  };

}

#endif
