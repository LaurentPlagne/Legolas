#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include <list>

#include <vector>
#include "Legolas/Vector/FlatMultiVector.hxx"

using Legolas::SizeType;

int main( int argc,  char *argv[] )
  
{


  {//Level=3 string
    INFOS("****** Build rectangular shape from string ********");
    INFOS("******* The Shape defines a 3D vector that contains 3 1D vector of size 2");
    Legolas::MultiVector<double,3>::Shape shape("(2x(3x4))");

    std::vector< std::vector<int> > als=Legolas::buildLevelSizes(shape);
    Legolas::displayLevelSizes(als);


    Legolas::FlatMultiVector<double,3> Xf(shape);

    int value=1.0;

    for (int l3=0 ; l3<Xf.size() ; l3++){
      for (int l2=0 ; l2<Xf[l3].size() ; l2++){
	for (int l1=0 ; l1<Xf[l3][l2].size() ; l1++){

	  Xf[l3][l2][l1]=value;
	  value++;
	}
      }
    }

    INFOS("Xf="<<Xf);

    // Copy Ctor Test
    Legolas::FlatMultiVector<double,3> Xfbis(Xf);

    INFOS("Xfbis="<<Xfbis);

    // Assign Operator Test
    Legolas::FlatMultiVector<double,3> Xfter(Xf.shape());
    Xfter=Xfbis;

    INFOS("Xfter="<<Xfter);

    // Scalar Assign Operator;
    
    Xfter[1]=1.0;

    INFOS("Xfter="<<Xfter);

    Xfter[1]+=1.0;

    INFOS("Xfter="<<Xfter);

    Xfter[1]-=1.0;

    INFOS("Xfter="<<Xfter);

    Xfter[1]*=2.0;

    INFOS("Xfter="<<Xfter);

  }

 
  if (true)
  {
    // Exemple et test pour la construction d'1 vecteur multidim non rectangulaire.
    // On veut definir un vecteur X à 4 dimensions :
    // X[ 0:nGroup ][ 0:nHarmonic ][0][0:jxSize]
    // X[ 0:nGroup ][ 0:nHarmonic ][1][0:jySize]
    // X[ 0:nGroup ][ 0:nHarmonic ][2][0:jzSize]
    // X[ 0:nGroup ][ 0:nHarmonic ][3][0:fluxSize]

    int jxSize=2; int jySize=3; int jzSize=3;
    int fluxSize=4;
    
    // SpatialShape (not rectangular)

    Legolas::FlatMultiVector<double,2>::Shape spatialShape;
    spatialShape.appendElement(jxSize);
    spatialShape.appendElement(jySize);
    spatialShape.appendElement(jzSize);
    spatialShape.appendElement(fluxSize);

    // Harmonic Level (rectangular)
    int nHarmonic=2;

    Legolas::FlatMultiVector<double,3>::Shape harmonicShape(nHarmonic,spatialShape);

    // Group Level (rectangular)
    int nGroup=2;

    Legolas::FlatMultiVector<double,4>::Shape groupShape(nGroup,harmonicShape);
    
    INFOS("groupShape="<<groupShape);
    
    Legolas::RectangularMultiVector<double,4> X(groupShape);
    X=1.0;
    INFOS("X="<<X);

    Legolas::RectangularMultiVector<double,4> Y(groupShape);
    Y=2.0;

    Legolas::swap(X,Y);

    INFOS("X="<<X);

    

    
  }



}




    
