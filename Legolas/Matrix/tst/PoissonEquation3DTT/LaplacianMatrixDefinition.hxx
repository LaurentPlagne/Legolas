/**
 * project  DESCARTES
 *
 * @file     LaplacianMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_LAPLACIANMATRIXDEFINITION_HXX__
#define __LEGOLAS_LAPLACIANMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"

template <class RT>
class LaplacianMatrixDefinition  : public Legolas::DefaultMatrixDefinition<RT> {
public:
  //  Types that must be defined to model the MATRIX_DEFINITION concept 

  typedef Legolas::TriDiagonal  MatrixStructure;
  typedef RT                    RealType;
  typedef RT                    GetElement;

  class Data : public Legolas::MatrixShape<1>{
  public:
    Data( const Legolas::Vector<RealType> & mesh):Legolas::MatrixShape<1>(mesh.size()-2,mesh.size()-2){
      RealType h=mesh[1]-mesh[0];
      h2m1_=1.0/(h*h);
    }

    RealType h2m1( void ) const { return h2m1_;}
    
  private:
    RealType h2m1_;
  };
  

  //  3 static functions to be defined to model the TRIDIAGONAL_MATRIX_DEFINITION concept 

  static inline GetElement diagonalGetElement( int i , const Data & data) { return -2.0*data.h2m1();}
  static inline GetElement upperDiagonalGetElement( int i , const Data & data) { return 1.0*data.h2m1();}
  static inline GetElement lowerDiagonalGetElement( int i , const Data & data) { return 1.0*data.h2m1();}

};


#endif
    

									     

