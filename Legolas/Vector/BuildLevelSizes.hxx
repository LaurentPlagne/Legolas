#pragma once

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Shape/L1Shape.hxx"
#include <vector>


using Legolas::SizeType;

namespace Legolas{

  inline void displayLevelSizes(const std::vector< std::vector<int> > & ls){
    for (uint l=0 ; l < ls.size() ; l++){
      for (uint i=0 ; i < ls[l].size() ; i++){
	INFOS("ls["<<l<<"]["<<i<<"]="<<ls[l][i]);
      }
    }
  }


  template <class SHAPE>
  inline std::vector< std::vector<int> > buildLevelSizes(const SHAPE & shape){
    int level=shape.level();
    std::vector< std::vector<int> > allLevelSizes(level);
    for (int l=1 ; l<level ; l++){
      recursiveAppendLevel(shape,allLevelSizes[l],l);
    }

    //    displayLevelSizes(allLevelSizes);

    return allLevelSizes;
  }

  //level=1;
  inline std::vector< std::vector<int> > buildLevelSizes(const int & shape){
    
    std::vector< std::vector<int> > allLevelSizes(2);
    allLevelSizes[1].push_back(shape);
    
    return allLevelSizes;
  }

  


  
  template <class SHAPE>
  inline void recursiveAppendLevel(const SHAPE & shape,
				   std::vector<int> & thisLevelSizes,
				   int thisLevel){
    int shapeLevel=shape.level();
    //    INFOS("shapeLevel="<<shapeLevel);
    //    INFOS("shape.size()="<<shape.size());
    //    INFOS("thisLevel=="<<thisLevel);
    if (thisLevel==(shapeLevel-1)){

      for (int i=0 ; i< shape.size() ; i++){
	//	INFOS("ICI");
	//	thisLevelSizes.push_back(Legolas::getSize(shape[i]));
	thisLevelSizes.push_back(shape[i].size());
      }
    }
    else{
      for (int i=0 ; i< shape.size() ; i++){
	recursiveAppendLevel(shape[i],thisLevelSizes,thisLevel);
      }
    }
  }


  // template <>
  // inline void recursiveAppendLevel<int>(const int & shape,
  // 					std::vector<int> & thisLevelSizes,
  // 					int thisLevel){
  //   return;
  // }


  inline void recursiveAppendLevel(const L1Shape & shape,
				   std::vector<int> & thisLevelSizes,
				   int thisLevel){
    return;
  }



}

  
