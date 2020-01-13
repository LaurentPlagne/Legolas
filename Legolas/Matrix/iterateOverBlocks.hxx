#ifndef __LEGOLAS_ITERATEOVERBLOCKS_HXX__
#define __LEGOLAS_ITERATEOVERBLOCKS_HXX__

#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Matrix/MatrixStream.hxx"

namespace Legolas{

  void iterateOverBlocks(const AnyMatrix & anyMatrix,
			 Legolas::MatrixStream & aStream){
    anyMatrix.iterateOverBlocksMethod(aStream);
  }

  void iterateOverBlocks(const double & aDouble,
			 Legolas::MatrixStream & aStream){
    aStream.dumpElement(aDouble);
  }

  void iterateOverBlocks(const float & aFloat,
			 Legolas::MatrixStream & aStream){
    aStream.dumpElement(aFloat);
  }
  
}

#endif
  
