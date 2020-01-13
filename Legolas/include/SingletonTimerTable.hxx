/**
 * project  DESCARTES
 *
 * @file     SingletonTimerTable.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef SINGLETONTIMERTABLE_HXX
#define SINGLETONTIMERTABLE_HXX

#include <pthread.h>

#include "UTILITES.hxx"
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class SingletonTimerTable{

public:

  typedef std::map< std::string , std::vector<double> > MeasurementType;
  pthread_mutex_t mutexsum;

  SingletonTimerTable( void ):_measurements(),_cumulate_measurement(0.0)
  {
    pthread_mutex_init(&mutexsum, NULL);
    //INFOS("SingletonTimerTable Default Ctor");
  }

  ~SingletonTimerTable( void )
  {
    pthread_mutex_destroy(&mutexsum);
    //INFOS("Dtor");

    MeasurementType::iterator it;
    
    for (it=_measurements.begin(); it!=_measurements.end() ; it++){

      std::cout<<"**************************************************"<<std::endl;
      std::cout<<"*operation name : " << (*it).first <<std::endl;
      std::cout<<"*  nb_measurement = " << (*it).second.size() <<std::endl;

      //      int MeasurementCounter=(*it).second.size() ;
      
      double minTime=(*it).second[0];

      double totalTime=0.0;

      for (unsigned int i=0 ; i< (*it).second.size() ; i++){
	double individualTime=(*it).second[i];
	totalTime+=individualTime;
	if (minTime>individualTime) minTime=individualTime;
      }

      double averageTime=totalTime/double((*it).second.size());
      
      std::cout<<"*  total time   = " << totalTime <<std::endl;
      std::cout<<"*  average time/call   = " << averageTime <<std::endl;
      std::cout<<"*  minimal time/call   = " << minTime<<std::endl;
      

    }
    if (_measurements.begin() !=_measurements.end()) {
	std::cout<<"**************************************************"<<std::endl;
	std::cout<<"Anonymous cumulated time (s)  = " <<  _cumulate_measurement <<std::endl;
    }
    
    
  }

  void add_measurement( const std::string & operation_name , double operation_time ){
    pthread_mutex_lock (&mutexsum);
    _measurements[operation_name].push_back(operation_time);
    pthread_mutex_unlock (&mutexsum);
  }

  void cumulate_measurement(double operation_time){
    _cumulate_measurement+=operation_time;
  }

private:

  MeasurementType _measurements;
  double _cumulate_measurement;

};
#endif
