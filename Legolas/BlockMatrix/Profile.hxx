#ifndef __PROFILE_HXX__
#define __PROFILE_HXX__

#include <map>

namespace Legolas{

  struct OperationProfile{
    double duration;
    size_t counter;
    OperationProfile( void ):duration(0.0),counter(0){}

  };

//   struct Contribution{
//     double duration;
//     size_t counter;
//     double percent;
//     Contribution( void ):duration(0.0),counter(0),percent(0){}
//   };

  struct CompoundBlockProfile{
    std::map< std::string, OperationProfile > operationProfiles;
    std::map< std::string, std::map< std::string, OperationProfile > > contributions;
    //    std::map< std::string, std::map< std::string, Contribution > > contributions;
  };


  // Profile[Level L]["MatrixName"].operationProfiles["OperationName"].duration
  // ..................................................................counter
  // ...............................contributions["SubMatrixName"]["OperationName"].duration
  // ...............................contributions["SubMatrixName"]["OperationName"].counter

  typedef std::map< uint , std::map< std::string, CompoundBlockProfile> > Profile;


  inline std::string operationProfileString( std::map< std::string, OperationProfile >::const_iterator operationProfilePtr, double totalDuration){
    std::string result(".");
    result+=operationProfilePtr->first;
    result+=" [";
    result+=stringConvert(operationProfilePtr->second.duration);
    result+="s|x";
    result+=stringConvert(operationProfilePtr->second.counter);
    result+="|";
    result+=stringConvert(100.0*operationProfilePtr->second.duration/totalDuration);
    result+="%]";
    return result;
  }
  
  inline void dumpAndBlank( std::string & s){
    std::cout << s ;
    for (uint i=0 ; i<s.size() ; i++ ) s[i]=('-');
  }

  inline void dispProfile(Profile & profile){
    
    //Level Loop
    Profile::const_iterator it1=profile.begin();
    for (;it1!=profile.end();it1++){

      std::string levelString;
      
      levelString+="LEVEL ";
      levelString+=stringConvert((*it1).first);
      levelString+=":";
      //Matrix Name Loop
      const std::map< std::string, CompoundBlockProfile> & profileI=(*it1).second;
      std::map< std::string, CompoundBlockProfile>::const_iterator it2=profileI.begin();
      for (;it2!=profileI.end();it2++){

	
	std::string matrixString((*it2).first);

	//Operation Loop (max 2 and usually one is dominant)
	//  Step 1 : Compute total time of this matrix relatd operations :

	const std::map< std::string, OperationProfile > & ops=(*it2).second.operationProfiles;
	std::map< std::string, OperationProfile >::const_iterator it3=ops.begin();
	
	double totalDuration=0.0;
	for (;it3!=ops.end();it3++){
	  totalDuration+=(*it3).second.duration;
	}

	// Step 2 : Form the operations string to be shown :

	for (it3=ops.begin();it3!=ops.end();it3++){
	  //	  (*it3).first;
	  std::string operationString=operationProfileString(it3,totalDuration);
	  dumpAndBlank(levelString);
	  dumpAndBlank(matrixString);
	  dumpAndBlank(operationString);
	  std::cout << std::endl;
	}
	

	double subDuration=0.0;

	const std::map< std::string, std::map< std::string, OperationProfile > > & cbs=(*it2).second.contributions;
	std::map< std::string, std::map< std::string, OperationProfile > >::const_iterator it4=cbs.begin();
	for (;it4!=cbs.end();it4++){
	  const std::map< std::string, OperationProfile > & cbsop=(*it4).second;
	  std::map< std::string, OperationProfile >::const_iterator it5=cbsop.begin();
	  for (;it5!=cbsop.end();it5++){
	    subDuration+=it5->second.duration;
	  }
	}
	
	std::string contributionString(" ");
	contributionString+=stringConvert(100.0*subDuration/totalDuration);
	contributionString+="% in suboperators ";
	
	//	if (cbs.begin()==cbs.end()) INFOS("NO CONTRIBUTORS :LEVEL="<<(*it1).first);

	for (it4=cbs.begin();it4!=cbs.end();it4++){
	  
	  std::string subMatrixString((*it4).first);

	  const std::map< std::string, OperationProfile > & cbsop=(*it4).second;
	  std::map< std::string, OperationProfile >::const_iterator it5=cbsop.begin();
	  for (;it5!=cbsop.end();it5++){
	    std::string subOperationString=operationProfileString(it5,totalDuration);

	    dumpAndBlank(levelString);
	    dumpAndBlank(matrixString);
	    dumpAndBlank(contributionString);
	    dumpAndBlank(subMatrixString);
	    dumpAndBlank(subOperationString);
	    std::cout << std::endl;
	  }
	}

      }

    }


  }
      
			     
}

#endif

    

    

  
