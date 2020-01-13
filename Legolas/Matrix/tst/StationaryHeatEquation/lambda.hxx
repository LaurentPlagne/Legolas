#ifndef __LEGOLAS_LAMBDA_HXX__
#define __LEGOLAS_LAMBDA_HXX__

double  lambda(double x, double y) {
  double xc=0.5;
  double yc=0.5;
  double r0=0.2;
  double l0=1.;
  double l1=100.;

  /* one disc (l1 inside, l0 outside) */
  if ((x-xc)*(x-xc)+(y-yc)*(y-yc)<=r0*r0) {
    return(l1);
  }
  else {
    return(l0);
  }
}

#endif
