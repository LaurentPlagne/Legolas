/**
 * project  DESCARTES
 *
 * @file     GenericMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_GENERICMATRIXINTERFACE_HXX__
#define __LEGOLAS_GENERICMATRIXINTERFACE_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/Helper/Default.hxx"
#include "Legolas/Matrix/MatrixInterface/GenericMatrixInterfaceTraits.hxx"
//#include "Legolas/Matrix/MatrixVectorOperations.hxx"

namespace Legolas{

  template <class MATRIX_DEFINITION,
	    class MATRIX_OPTIONS=Default,
	    class MATRIX_ELEMENT_INTERFACE=Default>
  class GenericMatrixInterface: public GenericMatrixInterfaceTraits<MATRIX_DEFINITION,
								    MATRIX_OPTIONS,
								    MATRIX_ELEMENT_INTERFACE>::MatrixInterface
  {
  private:
    typedef GenericMatrixInterfaceTraits<MATRIX_DEFINITION,
					 MATRIX_OPTIONS,
					 MATRIX_ELEMENT_INTERFACE> GMIT;
  
    typedef typename GMIT::MatrixInterface  BaseClass;

  public:
  
    typedef MATRIX_ELEMENT_INTERFACE               MatrixElementInterface;
    typedef typename GMIT::MatrixDefinition        MatrixDefinition;  
    typedef typename GMIT::MatrixOptions           MatrixOptions;
    typedef typename GMIT::MatrixInterface::Solver Solver;

  };

}

#endif
