#ifndef __SPACEMATRIXTEST__
#define __SPACEMATRIXTEST__

#include "UTILITES.hxx" 

#include "SpaceVectorPrivateData.hxx"
#include "SpaceSourceVectorDefinition.hxx"
#include "SpaceExactVectorDefinition.hxx"
#include "SpaceTemperatureVectorDefinition.hxx"

#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "DSpaceMatrixDefinition.hxx"
#include "BSpaceMatrixDefinition.hxx"

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"

template <class REAL_TYPE>
class SpaceMatrixTest{
public:

  typedef REAL_TYPE RealType;

  SpaceMatrixTest( void ){
    MESSAGE("SpaceMatrixTest Ctor");
  }

  void apply( RealType deltat ) const{

    
    typedef DSpaceMatrixDefinition<RealType> MatrixDefinition;

    {

      typedef Actual StorageMode;
      typedef InputMatrixOptions<StorageMode> MatrixOptions;
      typedef GenericMatrixInterface< MatrixDefinition, MatrixOptions, RealElementInterface<RealType> > MatrixInterface;
      
      typedef typename MatrixInterface::Matrix Matrix;
      typedef typename MatrixInterface::Data data(deltaT);
      
      Matrix A(data);

      A(i,j);

    }

  }

};

#endif

      
  

