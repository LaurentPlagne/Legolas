#ifndef __DISPLAY_GP__
#define __DISPLAY_GP__

#include <string>
#include <fstream>

using namespace std;

template <class VECTOR,class REAL_TYPE, class INTEGER_TYPE>
void displayGP(const VECTOR & X, const REAL_TYPE & meshLenght, const REAL_TYPE & duration,const INTEGER_TYPE & gridSize, std::string filename){
  
  ofstream outfile (filename.c_str(),ios::out) ;

  // REAL_TYPE denominator=REAL_TYPE(gridSize+1);
  // REAL_TYPE deltaX=meshLenght/denominator;
  // REAL_TYPE deltaT=duration/denominator;

  for (int i=0 ; i < X.size() ; i++){
    for (int j=0 ; j < X[i].size() ; j++){
      
      outfile << i << " " << j << " " <<X[i][j] << endl;

    }
  }

}

#endif
      

