#pragma once


#include <Eigen/Core>

namespace Legolas{

  template <class T, int PACK_SIZE, int P2, int P3>
  double dot(const Eigen::Array<T, PACK_SIZE, P2, P3, PACK_SIZE,P2> & right,
	     const Eigen::Array<T, PACK_SIZE, P2, P3, PACK_SIZE,P2> & left){

    // double sum=0.0;
    // for (int i=0 ; i<PACK_SIZE ; i++){
    //   sum+=double(right(i))*double(left(i));
    // }

    //return sum;
    return right.matrix().dot(left.matrix());
  }

  //  template <class A>
  //  void  assignValueToScalar(double value, A & target){
  //    INFOS("ICI");
  //  }

  //  void assignValueToScalar(double value , Eigen::Array<float, 4, 1, 0, 4, 1> & target){
  //    INFOS("TOTO");
  //  }

  template <class T, int PACK_SIZE, int P2, int P3>
  void assignValueToScalar(double value,
  			   Eigen::Array<T, PACK_SIZE, P2, P3, PACK_SIZE,P2>  & target){
    typedef Eigen::Array<T, PACK_SIZE, P2, P3, PACK_SIZE,P2> ThisType;
    const ThisType packedValue(ThisType::Constant(value));
    //const Eigen::Array<T, PACK_SIZE, P2, P3, PACK_SIZE,P2> packedValueConstant(value) ;
    //target=Eigen::Array<T, PACK_SIZE, 1>::Constant(value) ;
    target=packedValue;
    //target=ThisType::Constant(value);
  }

  template <class T, int PACK_SIZE, int P2, int P3>
  std::ostream& operator<< ( std::ostream& os,
			     const Eigen::Array<T, PACK_SIZE, P2, P3, PACK_SIZE,P2> & v ){
    os << "|" ;
    for (int i=0 ; i<PACK_SIZE ; i++){
      os << v(i) ;
      if (i!=PACK_SIZE-1) os <<" ";
    }
    os << "|" ;
    return os ;
  }

  template <class T, int PACK_SIZE, int P2, int P3>
  void inverse(Eigen::Array<T, PACK_SIZE, P2, P3, PACK_SIZE,P2> & a){a=a.inverse();}


}
