/**
****************************************************************************
* <P>
* The mini-SSE-L1-BLAS Library is providing the basic Level 1 BLAS
* operations on vectors. In particular, the four functions: SDOT,
* DDOT, SAXPY, DAXPY were deeply hand-optimized. The library is
* composed of only 2 files (a ".cpp" and a ".h" file). It is
* cross-platform. It compiles transparently on both Visual Studio
* .NET and GCC. A FORTRAN interface is also available. Inclusion of
* the library inside your own projects is easy. Optimization
* includes inline SSE and SSE2 assembly code inserted inside the
* C/C++ code, loop unrolling. Assembly code instructions are ordered
* to increase parallel execution of instructions, to ease branch
* prediction, to reduce dependency links between two close
* instructions. Software Prefetch instructions were very roughly
* investigated but were not included in the library. The functions
* are also implemented as C++ macros to remove the function calls
* overhead.
* </P>
*
* @version     V1.03
*
* @author      Frank Vanden Berghen
*
* Copyright (c) 2002, Frank Vanden Berghen
* All rights reserved.
* See the file "AFPL-license.txt" about the licensing terms
*
****************************************************************************
*/

#ifndef _TIME_MACRO_H_
#define _TIME_MACRO_H_

#include <stdio.h>

#ifdef WIN32

    #include <windows.h>
    static double mydtime()
    {
        // return (double)GetTickCount() * 1.0e-03;
        LARGE_INTEGER c; QueryPerformanceCounter(&c);
        LARGE_INTEGER f; QueryPerformanceFrequency(&f);
        return ((double)(c.QuadPart))/f.QuadPart;
    };

#else

    #include <sys/time.h>
    #include <sys/resource.h>
    #include <unistd.h>
    #ifdef __hpux
    #include <sys/syDOTl.h>
    #endif

    struct rusage rusage;
    double mydtime()
    {
        getrusage(RUSAGE_SELF,&rusage);
        return (double)(rusage.ru_utime.tv_sec)+(double)(rusage.ru_utime.tv_usec) * 1.0e-06;
    }

#endif

typedef struct BenchTimeTag {
 public:

    double startd,sum,mmin,mmax;
    long nloop;

    BenchTimeTag(){ reset(); }

    inline void start() { startd=mydtime(); }
    inline void stop()
    {
        double stop= mydtime() - startd;
        sum+=stop;
        if (stop<mmin) mmin=stop;
        if (stop>mmax) mmax=stop;
        nloop++;
    }
    inline void reset() { sum=0; mmin=1e30; mmax=0; nloop=0; }
    inline void print(const char *title, FILE *f=stdout)
    {
        if(nloop) fprintf(f,"%s total:%7d, mean:%5d, min:%5d, max:%5d, loop:%2d\n",
                    title,
                    (int)(sum*1000),
                    (int)(sum/nloop*1000),
                    (int)(mmin*1000),
                    (int)(mmax*1000),
                    (int)(nloop));
    }
} BenchTime;



#endif

