#include "UTILITES.hxx"
#include "Legolas/Vector/Shape/CompressedVector.hxx"

#include <list>

int main( int argc,  char *argv[] )
{
  
  typedef unsigned int Index;
  

  // Tests 1D
  {
    //Default Ctor
    std::cout << "** Test 1D CompressedVector Default Ctor + elementary append " << std::endl ; 
    Legolas::CompressedVector<Index> cv;
    
    cv.appendElement(1);
    cv.appendElement(2);
    cv.appendElement(3);

    std::cout << cv << std::endl ;

    //Copy Ctor
    std::cout << "** Test 1D CompressedVector Copy Ctor" << std::endl ; 
    Legolas::CompressedVector<Index> cv2(cv);
    cv2.appendElement(4);
    std::cout << cv2 << std::endl ;
  }

  {
    //NElement Ctor
    std::cout << "** Test 1D CompressedVector N Elements Ctor " << std::endl ; 
    Legolas::CompressedVector<Index> cv(3,2);
    std::cout << cv << std::endl ;
  }

  {
    //List Ctor
    std::list<Index> alist;
    alist.push_back(1);
    alist.push_back(1);
    alist.push_back(2);    
    alist.push_back(2);    
    alist.push_back(2);    
    alist.push_back(3);
    Legolas::CompressedVector<Index> cv(alist);
    std::cout << "** Test 1D CompressedVector List Ctor " << std::endl ; 

    std::cout << cv << std::endl ;
  }

  {
    //String Ctor
    Legolas::CompressedVector<Index> cv("(23x2,12x34,1x45)");
    std::cout << "** Test 1D CompressedVector String Ctor " << std::endl ; 

    std::cout << cv << std::endl ;
  }
  

  // Tests 2D
  {
    //Default Ctor
    std::cout << "** Test 2D CompressedVector Default Ctor + elementary append " << std::endl ; 

    Legolas::CompressedVector<Index> cv1D1;
    
    cv1D1.appendElement(1);
    cv1D1.appendElement(2);
    cv1D1.appendElement(2);
    cv1D1.appendElement(3);

    Legolas::CompressedVector<Index> cv1D2;
    
    cv1D2.appendElement(1);
    cv1D2.appendElement(2);
    cv1D2.appendElement(3);

    Legolas::CompressedVector< Legolas::CompressedVector<Index> > cv2D1;

    cv2D1.appendElement(cv1D1);
    cv2D1.appendElement(cv1D2);
    cv2D1.appendElement(cv1D2);

    std::cout << cv2D1 << std::endl ;

    //Copy Ctor

    std::cout << "** Test 2D CompressedVector Copy Ctor" << std::endl ; 
    Legolas::CompressedVector< Legolas::CompressedVector<Index> > cv2D2(cv2D1);

    cv2D2.appendElement(cv1D1);
    std::cout << cv2D2 << std::endl ;

    
    //String Ctor
    std::cout << "** Test 2D CompressedVector String Ctor " << std::endl ; 
    Legolas::CompressedVector< Legolas::CompressedVector<Index> > cv2D3("(1x(1x1,2x2,1x3),2x(1x1,1x2,1x3),1x(1x1,2x2,1x3),1x(1x1,2x2,1x3))");

    std::cout << cv2D3 << std::endl ;

  }

  // Tests 3D
  {
    
    //String Ctor
    std::cout << "** Test 3D CompressedVector String Ctor " << std::endl ; 
    typedef Legolas::CompressedVector<Index> CV1D;
    typedef Legolas::CompressedVector<CV1D>  CV2D;
    typedef Legolas::CompressedVector<CV2D>  CV3D;

    CV3D cv3D1("(2x(1x(1x1,2x2,1x3),2x(1x1,1x2,1x3),1x(1x1,2x2,1x3),1x(1x1,2x2,1x3)),1x(1x(1x1,2x2,1x3),2x(1x1,1x2,1x3),1x(1x1,2x2,1x3),1x(1x1,2x2,1x3)))");

    std::cout << cv3D1 << std::endl ;

    CV3D cv3D2("(2x(3x(4x5)))");

    std::cout << cv3D2 << std::endl ;


  }
  INFOS("FIN DU TEST");
    

}


    
