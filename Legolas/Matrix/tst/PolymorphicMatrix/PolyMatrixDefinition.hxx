#ifndef __LEGOLAS_POLYMATRIXDEFINITION_HXX__
#define __LEGOLAS_POLYMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
#include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "Legolas/Matrix/PolymorphicMatrix.hxx"

#include "DenseMatrixDefinition.hxx"
#include "DiagonalMatrixDefinition.hxx"

#include <memory>

class PolyMatrixDefinition : public Legolas::DefaultMatrixDefinition<double>{
public:
 //  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::Dense              MatrixStructure;
  typedef double                      RealType;
  typedef Legolas::PolymorphicMatrix  GetElement;    

  typedef Legolas::MatrixShape<2> Data;

private:
  typedef Legolas::InputMatrixOptions<Legolas::Virtual>     IMO;
  typedef Legolas::RealElementInterface<double>  EI ;
  typedef Legolas::GenericMatrixInterface<DenseMatrixDefinition,IMO,EI> GMIDense;
  typedef Legolas::GenericMatrixInterface<DiagonalMatrixDefinition,IMO,EI> GMIDiagonal;

public:

  static inline GetElement getElement( int i , int j, const Data & data) { 

    if (i==j){
      std::shared_ptr<Legolas::AnyMatrix>  elementPtr(new GMIDense::Matrix(data.getSubMatrixShape(i,j)));
      return GetElement(elementPtr);
    }
    else{
      std::shared_ptr<Legolas::AnyMatrix>  elementPtr(new GMIDiagonal::Matrix(data.getSubMatrixShape(i,j)));
      return GetElement(elementPtr);
    }

  }


};
#endif
    

									     

