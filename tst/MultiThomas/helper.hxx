#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

inline std::vector<int> makeSizes( void ){
    const int firstPowerOfTwo=8;
    const int lastPowerOfTwo=512;
    // const int firstPowerOfTwo=64;
    // const int lastPowerOfTwo=128;
    const int nbPointsBetweenPowerOfTwo=7;

    std::vector<int> result;

    int powerOfTwo=firstPowerOfTwo;
    while (powerOfTwo<lastPowerOfTwo){
        const int nextPowerOfTwo=powerOfTwo*2;
        const int shift=(nextPowerOfTwo-powerOfTwo)/nbPointsBetweenPowerOfTwo;
        int s=powerOfTwo;
        for (int i=0 ; i< nbPointsBetweenPowerOfTwo-1 ; i++){
            result.push_back(s);
            s+=shift;
        }
        powerOfTwo=nextPowerOfTwo;
    }
    result.push_back(powerOfTwo);

    for (size_t i=0 ; i<result.size(); i++){
        std::cout << "sizes["<<i <<"]="<<result[i] << std::endl;
    }
    return result;
}



inline void savePerf(const std::vector<int> & sizes,
                     const std::vector<double> & perfs,
                     const std::string filename){
     std::ofstream myfile;
     myfile.open(filename.c_str());
     for (size_t i=0 ; i<sizes.size() ; i++){
       myfile << sizes[i] <<" "<< perfs[i] << std::endl;
     }
     myfile.close();
}

template <class A2D>
void display2D(A2D X2D){
  for (int j=0 ; j<3 ; j++){
    std::cout << "[";
    for (int i=0 ; i<3 ; i++){
      std::cout << X2D[j][i]<< " ,";
    }
    std::cout <<"...,";
    for (int i=X2D[j].size()-4 ; i<X2D[j].size() ; i++){
      std::cout << X2D[j][i];
      if (i!=(X2D[j].size()-1)) std::cout <<",";
    }
    std::cout <<"]"<<std::endl;
  }
  std::cout<<"...."<<std::endl;
  for (int j=X2D.size()-4; j<X2D.size() ; j++){
    std::cout << "[";
    for (int i=0 ; i<3 ; i++){
      std::cout << X2D[j][i]<< " ,";
    }
    std::cout <<"...,";
    for (int i=X2D[j].size()-4 ; i<X2D[j].size() ; i++){
      std::cout << X2D[j][i];
      if (i!=(X2D[j].size()-1)) std::cout <<",";
    }
    std::cout <<"]"<<std::endl;
  }
}

