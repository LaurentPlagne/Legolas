#ifndef THOMASSOLVER_HXX
#define THOMASSOLVER_HXX

struct ThomasSolver{
  template <class A1D>
  void operator()(A1D D, A1D U, A1D L,
                  A1D B, A1D X) const {
//    typedef typename A1D::Element Element;
    typedef typename A1D::RealType  Scalar;
    A1D S(D.shape());
    Scalar  one(1.0),s,sm1;

    s=D[0];
    sm1=one/s;
    const int size=X.size();
    //forward
    X[0]=B[0]*sm1;
    for(int i=1; i<size; i++){
      S[i]=U[i-1]*sm1;
      s=D[i]-L[i]*S[i];
      X[i]=B[i]-L[i]*X[i-1];
      sm1=one/s;
      X[i]*=sm1;
    }
    //backward
    for (int i=(size-2);i>=0 ; i--){
      X[i]-=S[i+1]*X[i+1];
    }
  }

};

#endif // THOMASSOLVER_HXX
