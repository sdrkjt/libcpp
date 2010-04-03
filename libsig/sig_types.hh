#ifndef sig_types_hh
#define sig_types_hh

#include <complex>
//#include <xmmintrin.h>
#include <emmintrin.h>  
#include "AlignedArray.hh"
#include "sig_complexd.hh"
#include "sig_complex.hh"
#include "sig_v2Cxs.hh"

/*---------INTEGER----------------------------------------*/              
typedef unsigned char                               uint8  ;          //  Unsigned byte
typedef unsigned short                              uint16 ;          //  Unsigned word
//typedef unsigned long                             uint32 ;          //  Unsigned double word
typedef unsigned int                                uint32 ;
typedef unsigned long long                          uint64 ;          //  Unsigned quadruple word
			                            	   
typedef signed char                                  int8  ;          //  Signed byte
typedef signed short                                 int16 ;          //  Signed word
//typedef signed long                                int32 ;          //  Signed double word
typedef signed int                                   int32 ;
typedef signed long long                             int64 ;          //  Signed quadruple word

struct uint24 {
  uint8 tab[3];
};

/*---------COMPLEX----------------------------------------*/              
typedef std::complex<float >                      Complexs ;          //Cxs is defined in sig_v2Cxs.hh
typedef std::complex<double>                      Complexd ;          //Cxd is defined in sig_complexd.hh

typedef Cx<uint8 >                                Cxui8    ;
typedef Cx< int8 >                                Cxi8     ;
typedef Cx<uint16>                                Cxui16   ;
typedef Cx< int16>                                Cxi16    ;



/*---------SIG--------------------------------------------*/              
typedef AlignedArray<float ,__v4sf >               Sigfl   ;
typedef AlignedArray<double,__v2df >               Sigdb   ;
typedef AlignedArray<int   ,__v4si >               Sigi32  ;
typedef AlignedArray<int16 ,__v8hi >               Sigi16  ;
typedef AlignedArray<int8  ,__v16qi>               Sigi8   ;
							   
typedef AlignedArray<uint8 ,__v16qi>               Sigui8  ;
typedef AlignedArray<uint16,__v8hi >               Sigui16 ;
typedef AlignedArray<uint24,__v4si >               Sigui24 ;          //not valid yet
typedef AlignedArray<uint32,__v4si >               Sigui32 ;

typedef AlignedArray<__v16qi ,__v16qi>             Sigv16qi;

typedef AlignedArray<Cxd   ,  Cxd  >               Sigcxd  ;
typedef AlignedArray<Cxs   ,__v2Cxs>               Sigcxs  ;
typedef AlignedArray<Cxui8 ,__v16qi>               Sigcxui8;         //temporal solution, * and / unvalid
typedef AlignedArray<Cxi8  ,__v16qi>               Sigcxi8 ;         //temporal solution, * and / unvalid




//Sigdb, Sigcx
// typedef AlignedArray<float,__m128 >    Sigf;
// typedef AlignedArray<int  ,__m128i >    Sigi;
// typedef AlignedArray<short,__m128i >    Sigs;
// typedef AlignedArray<char ,__m128i>    Sigch;

/***************NOTE**************************
 *sizeof(char          ) = 1
 *sizeof(short         ) = 2
 *sizeof(int           ) = 4
 *sizeof(long          ) = 4
 *sizeof(unsigned char ) = 1
 *sizeof(unsigned short) = 2
 *sizeof(unsigned int  ) = 4
 *sizeof(unsigned long ) = 4
 *sizeof(float         ) = 4
 *sizeof(double        ) = 8
 *sizeof(void           *) = 4
 *sizeof(char           *) = 4
 *sizeof(short          *) = 4
 *sizeof(int            *) = 4
 *sizeof(long           *) = 4
 *sizeof(unsigned char  *) = 4
 *sizeof(unsigned short *) = 4
 *sizeof(unsigned int   *) = 4
 *sizeof(unsigned long  *) = 4
 *sizeof(float          *) = 4
 *sizeof(double         *) = 4
 *sizeof(int (*)()       ) = 4
 *********************************************/

#endif
