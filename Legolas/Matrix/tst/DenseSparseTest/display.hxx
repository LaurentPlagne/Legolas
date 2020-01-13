#ifndef __LEGOLAS_DISPLAY_HXX__

#include <iostream>

template <class MATRIX>
void display(const MATRIX & A){
  for (int i=0 ; i< A.nrows() ; i++){
    std::cout << "[" ;
    for (int j=0 ; j< A.ncols() ; j++){
      std::cout << A(i,j) << " " ;
    }
    std::cout << "]" << std::endl;
  }
}

template <class MATRIX>
void display2D(const MATRIX & A){
  for (int i=0 ; i< A.nrows() ; i++){
    for (int j=0 ; j< A.ncols() ; j++){
      std::cout << "i="<<i<<" j="<<j<<std::endl;
      display(A(i,j)) ;
    }
  }
}

#endif
