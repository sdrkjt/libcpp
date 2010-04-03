#ifndef TSC_hh
#define TSC_hh

#include "cpp_monit.hh"

#if ! defined(_MSC_VER) 
// define type __int64 if not defined
typedef long long __int64;
#endif

#ifdef ASM 
#include "asmlibran.h"
#else
__int64 ReadTSC() ;
#endif



class speed {
public:
  static __int64 last_TSC  ;
  static __int64 curr_TSC  ;
  static __int64 diff_TSC  ;
  static __int64 clock     ;
  
  static __int64 overhead  ;
};

#define RESET_TIME() speed::clock = 0;

#define TIME(name)							\
  speed::curr_TSC  = ReadTSC();						\
  speed::diff_TSC  = speed::curr_TSC-speed::last_TSC-speed::overhead;	\
  cout<<c_blue<<"   +   "						\
  <<double(speed::diff_TSC)<<c_green<<"   "<<string(name)<<c_normal<<endl; \
  speed::clock += speed::diff_TSC;	mytrace();			\
  speed::last_TSC  = ReadTSC();						\
									\
  
#define OVERHEAD()							\
  {									\
    __int64 macro_clocks[20];						\
    __int64 macro_before, macro_overhead;				\
    /* dummy test loop without test code to measure overhead */		\
    for (int i=0; i < 20; i++) {					\
      macro_before = ReadTSC();                   /*clock count before*/ \
      /*no test code here, measure overhead only*/			\
      macro_clocks[i] = ReadTSC() - macro_before;     /*clock count after*/ \
    }									\
    /* find minimum overhead */						\
    macro_overhead = macro_clocks[0];					\
    for (int i=0; i < 20; i++) {					\
      if (macro_clocks[i] < macro_overhead) macro_overhead = macro_clocks[i]; \
    }									\
    printf ("\n  overhead     clock cycles");				\
    for(int ii=0;ii<20;ii++)						\
      printf ("\n%6i  %14G", ii, double(macro_clocks[ii]));		\
    printf ("\n%6s  %14G","mean", double(macro_overhead));		\
    printf ("\n");							\
    speed::overhead = macro_overhead;					\
  }									\


int GET_TIME();


#endif
