#ifndef __BACKTRACE_HXX__
#define __BACKTRACE_HXX__
/*
 Come From:
 http://tombarta.wordpress.com/2008/08/01/c-stack-traces-with-gcc/

 WARNING:
 *Specific to gcc
 *Dont Forget -rdynamic option for the linker

 Small Modification :
 *'' => '\0'  For compilation
 * define as a macro
 * use std::cerr
 * static_cast
*/
#include <cstring>
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>

#define BACKTRACE \
{\
 const size_t max_depth = 100;\
 size_t stack_depth;\
 void *stack_addrs[max_depth];\
 char **stack_strings;\
\
 stack_depth = backtrace(stack_addrs, max_depth);\
 stack_strings = backtrace_symbols(stack_addrs, stack_depth);\
\
 std::cerr <<"Call stack from "<<__FILE__<<":"<<__LINE__<< " :"<<std::endl;\
 for (size_t i = 1; i < stack_depth; i++) {\
   size_t sz = 20000; /*just a guess, template names will go much wider*/\
   char *function = static_cast<char*>(malloc(sz));\
   char *begin = 0, *end = 0;\
   /*find the parentheses and address offset surrounding the mangled name*/\
   for (char *j = stack_strings[i]; *j; ++j) {\
     if (*j == '(') {\
       begin = j;\
     }\
     else if (*j == '+') {\
       end = j;\
     }\
   }\
   if (begin && end) {\
     *begin++ = '\0';\
     *end = '\0';\
     /* found our mangled name, now in [begin, end)*/\
\
     int status;\
     char *ret = abi::__cxa_demangle(begin, function, &sz, &status);\
\
     if (ret) {\
       /* return value may be a realloc() of the input*/\
       function = ret;\
     }\
     else {\
       /*demangling failed, just pretend it's a C function with no args*/\
       std::strncpy(function, begin, sz);\
       std::strncat(function, "()", sz);\
       function[sz-1] = '\0';\
     }\
     std::cerr << "\tStack #"<<i<<" " << stack_strings[i]<<":"<<function<<std::endl;	\
   }\
   else\
   {\
     /*didn't find the mangled name, just print the whole line*/\
     std::cerr<<"\tStack #"<<i<<" "<< stack_strings[i]<<std::endl;\
   }\
   free(function);\
 }\
}

#endif
