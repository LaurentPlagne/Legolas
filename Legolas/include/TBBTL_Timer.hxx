//=====================================================
// File   :  x86_timer.hh
// Author :  L. Plagne <laurent.plagne@edf.fr)>
// Copyright (C) EDF R&D,  mar déc 3 18:59:35 CET 2002
//=====================================================
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
#ifndef _TBBTL_TIMER_HH
#define _TBBTL_TIMER_HH

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/times.h>
//#include "system_time.h"
#include <asm/msr.h>
#include "UTILITES.hxx"
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include "SingletonTimerTable.hxx"

#include "my_tbb_parallel_for.hxx"

class TBBTL_Timer {

public :


  typedef my_tbb::tick_count ClickType;
  typedef std::map<ClickType,ClickType> CountedClickType;


  inline void improveAccuracy( void ){
    _residualTime=1.e10;
    for (int i=0 ; i < 100 ; i++){
      this->start();
      this->stop();
      const double this_time=this->get_raw_elapsed_time_in_second();
      if (this_time< _residualTime)  _residualTime=this_time;
    }
    //INFOS("_residualTime="<<_residualTime);
  }


  TBBTL_Timer( void ):_residualTime(0.0)
  {
    MESSAGE("TBBTL_Timer Default Ctor");
    //    this->improveAccuracy();
  }


  inline void start( void ){
    _click_start=my_tbb::tick_count::now();
  }


  inline void stop( void ){
    _click_stop=my_tbb::tick_count::now();
  }

  inline void storeResult(const std::string operation_name){
    _Table.add_measurement(operation_name,get_elapsed_time_in_second());
  }



  inline double get_elapsed_time_in_second( void ){
    //    INFOS("_residualTime="<<_residualTime);
    return (_click_stop-_click_start).seconds()-_residualTime;
  }

  inline double get_raw_elapsed_time_in_second( void ){
    //    INFOS("_residualTime="<<_residualTime);
    return (_click_stop-_click_start).seconds();
  }

private :

  double _residualTime;

  ClickType _click_start;
  ClickType _click_stop;

  static SingletonTimerTable _Table;


};


#endif
