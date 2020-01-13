/**
 * project  DESCARTES
 *
 * @file     ActualMatrixDataDriver.hxx
 *
 * @author Laurent PLAGNE
 * @date   Se
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */

#ifndef __LEGOLAS_ACTUALMATRIXDATADRIVERTRAITS_HXX__
#define __LEGOLAS_ACTUALMATRIXDATADRIVERTRAITS_HXX__

namespace Legolas{

  template <class MATRIX_CONTAINER_ENGINE, class MATRIX_STRUCTURE, class ELEMENT_DATA_DRIVER>
  class ActualMatrixDataDriverTraits{
  private:
  
    typedef MATRIX_CONTAINER_ENGINE  MatrixContainerEngine;
    typedef MATRIX_STRUCTURE         MatrixStructure;
    typedef ELEMENT_DATA_DRIVER      ElementDataDriver;

    typedef typename MatrixContainerEngine::Data        Data;
    typedef typename MatrixContainerEngine::ElementData ElementData;
    typedef ElementData &                               GetElement;
    typedef const ElementData &                         ConstGetElement;

    typedef typename MatrixStructure::template Engine<MatrixContainerEngine,
						      Data,
						      ConstGetElement,
						      GetElement,
						      ElementDataDriver> Engine;
  
    typedef typename Engine::Hierarchy                Hierarchy;
  
  public:

    class Type : public Hierarchy{
    public:
      static const int level=ElementDataDriver::level+1;

      template <class DENSE_INPUT_MATRIX>
      static inline void copy( const DENSE_INPUT_MATRIX & inputMatrix, 
			       typename Hierarchy::Data & matrix){
	Hierarchy::elementsCopy(inputMatrix,matrix);
	Hierarchy::setMaxIteration(matrix)=inputMatrix.getMaxIteration();
	Hierarchy::setEpsilon(matrix)=inputMatrix.getEpsilon();

	Hierarchy::setRowShape(matrix)=inputMatrix.getRowShape();
	Hierarchy::setColShape(matrix)=inputMatrix.getColShape();

      }

    };

  };

}

#endif
  
  
  
  
  

  


