/**
 * project  DESCARTES
 *
 * @file     Ex1.cxx
 *
 * @author Laurent PLAGNE
 * @date   september 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */

#ifndef __LEGOLAS_COMPARISON_HXX__
#define __LEGOLAS_COMPARISON_HXX__

// The template parameter must model the IsComparable concept.
// The IsComparable concept states that the < operator is defined  

template <class IS_COMPARABLE>
class Comparison{
public:
  static bool isSmaller(const IS_COMPARABLE & x, const IS_COMPARABLE & y){
    bool result=true;
    if (y<x) result=false;
    return result;
  }
};

#endif
