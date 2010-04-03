#ifndef cpp_monit_hh
#define cpp_monit_hh

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

/*------- difine colors-------------------------- */ 
#define c_normal           "\033[0m" 
#define c_black            "\033[30m"
#define c_red              "\033[31m"
#define c_green            "\033[32m"
#define c_yellow           "\033[33m"
#define c_blue             "\033[34m"
#define c_magenta          "\033[35m"
#define c_cyan             "\033[36m"
#define c_white            "\033[37m"
#define c_bblack           "\033[30;1m"
#define c_bred             "\033[31;1m"
#define c_bgreen           "\033[32;1m"
#define c_byellow          "\033[33;1m"
#define c_bblue            "\033[34;1m"
#define c_bmagenta         "\033[35;1m"
#define c_bcyan            "\033[36;1m"
#define c_bwhite           "\033[37;1m"
/*----------------------------------------------- */ 
using namespace std;

#define mygetchar() {cout<<c_magenta<<"<Stop In FILE: "<<__FILE__<<" At LINE: "<<__LINE__<<">"<<c_normal<<endl;getchar();}
#define mytrace()   cout<<c_magenta<<"<Trace In FILE: "<<__FILE__<<" At LINE: "<<__LINE__<<">"<<c_normal<<endl;
#define myabort()   {mytrace(); abort();}
#define DUMP(x)        cout<< #x << " = " << x <<endl;
#define DUMPN(x)       cout<< #x << " = \n " << x <<endl;
#define PRINT(name,val)cout<<name<<" "<<val<<"\n";

#define MSG(x)         cout<<c_blue    <<"MSG     :"<<c_normal<<x<<endl;
#define MSGc(x,color)  cout<<c_blue    <<"MSG     :"<<color   <<x<<c_normal<<endl;
#define MSG_WARNING(x) {cout<<c_magenta <<"WARNING :"<<c_normal<<x<<endl;mytrace();}
#define MSG_ERROR(x)   {cout<<c_bred     <<"ERROR   : "<<x<<c_normal<<endl;myabort();}

#define DUMPBIT(type,x)				 \
  {						 \
    char *pbyte = (char*)(&x);			 \
    int  nbbyte = sizeof(type);			 \
    cout<< #x << " = ";				 \
    for(int ibyte=nbbyte-1;ibyte>=0;ibyte--)	 \
    for(int ibit=7;ibit>=0;ibit--)		 \
      cout<<((pbyte[ibyte]>>ibit)&0x1);		 \
    cout<<"\n";					 \
  }						 \

  


#endif
