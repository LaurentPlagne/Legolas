/**
 * project  DESCARTES
 *
 * @file     X86Timer.cxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#include "dkconfig.hxx" /* pour récupérer USING_X86_TIMER */
#include "X86Timer.hxx"

#if defined( USING_X86_TIMER ) && USING_X86_TIMER == 1 /* macro définie dans dkconfig.hxx */

#include "SingletonTimerTable.hxx"

SingletonTimerTable X86Timer::_Table ;

#endif
