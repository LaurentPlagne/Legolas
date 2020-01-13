#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include <list>

using Legolas::SizeType;

int main( int argc,  char *argv[] )
{

#if 0
  {

    INFOS("ICI");
    
    size_t max31=2147483648;  //2**31;
    
    INFOS("max31="<<max31);
    
    //    size_t size=max31;  //2**31;

    size_t size=max31*2;  //2**31;

    INFOS("size="<<size);

    float * big = new float[size];

    //    for (size_t i=0 ; i<size ; i++) big[i]=double(i);

    delete[] big;
    
    INFOS("LA");
  }
#endif

#define LASUITE
#ifdef LASUITE

  {//Level=1
    INFOS("******* Show that Shape<1> is an int *********");
    const int Level=1;
    Legolas::MultiVector<double,Level>::Shape shape=2;
    //    int & anInt=shape;
    INFOS("shape="<<shape);
    //    INFOS("anInt="<<anInt);
    Legolas::MultiVector<double,Level> X(shape); 
    INFOS("X"<<X);
  }    
  {
    INFOS("******* L1 Short version *********");
    Legolas::MultiVector<double,1> X(2); 
    INFOS("X"<<X);
  }    

  {//Level=2
    INFOS("******* Build rectangular shape (long way) *********");
    INFOS("******* The Shape defines a 2D vector that contains 3 1D vector of size 2");
    Legolas::MultiVector<double,2>::Shape shape;
    Legolas::MultiVector<double,1>::Shape elementShape=2;//(int type)
    shape.appendElement(elementShape);
    shape.appendElement(elementShape);
    shape.appendElement(elementShape);
    INFOS("shape="<<shape);
    Legolas::MultiVector<double,2> X(shape); 
    INFOS("X="<<X);
  }

  {//Level=2 short
    INFOS("******* Build rectangular shape (short way) *******");
    INFOS("******* The Shape defines a 2D vector that contains 3 1D vector of size 2");
    Legolas::MultiVector<double,2>::Shape shape;
    Legolas::MultiVector<double,1>::Shape elementShape=2;
    const int outterSize=3;
    shape.appendNElement(outterSize,elementShape);
    INFOS("shape="<<shape);
    Legolas::MultiVector<double,2> X(shape); INFOS("X="<<X);
  }

  {//Level=2 string
    INFOS("****** Build rectangular shape from string ********");
    INFOS("******* The Shape defines a 2D vector that contains 3 1D vector of size 2");
    Legolas::MultiVector<double,2>::Shape shape("(3x2)");
    INFOS("shape="<<shape);
    Legolas::MultiVector<double,2> X(shape); 
    INFOS("X="<<X);
  }

  {//Level=2
    INFOS("******* Build non rectangular shape (long way) *********");
    INFOS("******* The Shape defines a 2D vector that contains 3 1D vector of sizes 2, 2 and 3");
    Legolas::MultiVector<double,2>::Shape shape;
    const int elementShape1=2;
    const int elementShape2=3;
    shape.appendElement(elementShape1);
    shape.appendElement(elementShape1);
    shape.appendElement(elementShape2);
    INFOS("shape="<<shape);
    Legolas::MultiVector<double,2> X(shape); 
    INFOS("X="<<X);
  }

  {//Level=2
    INFOS("******* Build non rectangular shape (short way) *******");
    INFOS("******* The Shape defines a 2D vector that contains 3 1D vector of sizes 2, 2 and 3");
    Legolas::MultiVector<double,2>::Shape shape;
    const int elementShape1=2;
    const int elementShape2=3;
    shape.appendNElement(2,elementShape1);
    shape.appendElement(elementShape2);
    INFOS("shape="<<shape);
    Legolas::MultiVector<double,2> X(shape); 
    INFOS("X="<<X);
  }

  {//Level=2
    INFOS("****** Build non rectangular shape from string ********");
    INFOS("******* The Shape defines a 2D vector that contains 3 1D vector of sizes 2, 2 and 3");
    Legolas::MultiVector<double,2> X("(2x2,1x3)"); 
    INFOS("X="<<X);
  }



  {
    // Exemple et test pour la construction d'1 vecteur multidim non rectangulaire.
    // On veut definir un vecteur X à 4 dimensions :
    // X[ 0:nGroup ][ 0:nHarmonic ][0][0:jxSize]
    // X[ 0:nGroup ][ 0:nHarmonic ][1][0:jySize]
    // X[ 0:nGroup ][ 0:nHarmonic ][2][0:jzSize]
    // X[ 0:nGroup ][ 0:nHarmonic ][3][0:fluxSize]

    //    int jxSize=2; int jySize=3; int jzSize=3;
    //    int fluxSize=4;
    
    /* le jeudi 15 septembre 2011 AY a supprimé un facteur 4;  le but est d'obtenir une taille totale inférieure à 4Go*/
    const size_t bigsize=289*289*84;
    INFOS("bigsize="<<bigsize);
    int jxSize=bigsize; int jySize=bigsize; int jzSize=bigsize;
    int fluxSize=bigsize;

    // SpatialShape (not rectangular)

    Legolas::RectangularMultiVector<double,2>::Shape spatialShape;
    spatialShape.appendElement(jxSize);
    spatialShape.appendElement(jySize);
    spatialShape.appendElement(jzSize);
    spatialShape.appendElement(fluxSize);
    spatialShape.appendElement(fluxSize);

    // Harmonic Level (rectangular)
    int nHarmonic=2;

    Legolas::RectangularMultiVector<double,3>::Shape harmonicShape(nHarmonic,spatialShape);

    // Group Level (rectangular)
    int nGroup=4;

    Legolas::RectangularMultiVector<double,4>::Shape groupShape(nGroup,harmonicShape);
    
    INFOS("groupShape="<<groupShape);
    
    Legolas::RectangularMultiVector<double,4> X(groupShape);
    INFOS("Avant assign");
    X=1.0;
    INFOS("Avant dot");
    INFOS("Legolas::dot(X,X)="<<Legolas::dot(X,X));

    X*=2.0;

    INFOS("apres * value");

    INFOS("Legolas::dot(X,X)="<<Legolas::dot(X,X));

    INFOS("la");

    //    Legolas::RectangularMultiVector<double,4> Xb(groupShape);
    
  }

#endif

  exit(0);
}




    
