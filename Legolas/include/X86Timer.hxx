//=====================================================
// File   :  x86_timer.hh
// Author :  L. Plagne <laurent.plagne@edf.fr)>        
// Modif : 64 bit compliant 2007
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
#ifndef _X86TIMER_HXX
#define _X86TIMER_HXX

#include "dkconfig.hxx" /* pour récupérer USING_X86_TIMER */
#include "SingletonTimerTable.hxx"

#if defined( USING_X86_TIMER ) && USING_X86_TIMER == 1 /* macro définie dans dkconfig.hxx */

#include "TBBTL_Timer.hxx"

//typedef TBBTL_Timer X86Timer
#define X86Timer TBBTL_Timer 

#else

#include <string>

class X86Timer {
public :
  inline X86Timer( void )
  {
  }

  inline void start( void ){
  }


  inline void stop( void ){
  }
  
  inline void storeResult(const std::string & operation_name){
  }
  inline double get_elapsed_time_in_second( void ){ return -1.0;}    

  
  inline void find_frequency( void ){
  }

  inline double frequency( void ){return -1.0 ;}

  inline void  add_get_click( void ){
  }    

  inline void dump_statistics(std::string filemane){
  }

  inline void dump_history(std::string filemane){
  }
     
private :
  static SingletonTimerTable _Table;
};

#endif // NOT USING_X86_TIMER



#endif
