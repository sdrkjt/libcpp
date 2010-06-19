#define check_asm 0
//g++ -S -O3 -msse2 t_sig_overhead.cc
#if !check_asm
#include "TSC.hh"
#else
#define TIME(x)
#define DUMP(x)
#define OVERHEAD()
#endif
   
#include <emmintrin.h>
#include <math.h>
#include "../libsig/libsig.hh"


#define ALIGNED __attribute__((  aligned(16)  ))
#if !check_asm
#define TLOOP(n) for(int il=0;il<n;il++){
#define ENDTLOOP() }
#else
#define TLOOP(n) 
#define ENDTLOOP() 
#endif


/************************************************************************/
#define Size 2048

#define TX   float//Cxs//Cxd//

#define VY    __m128//__v2Cxs//Cxd//

#define OPID +
#define FUNC add
string OPNAME = "ADD float";

/************************************************************************/

void test_sig_overhead(){
  //char* toto = new char[16];
  AlignedArray<TX,VY> cxa,cxb,cxc;
  cxa.ones(Size);cxb.ones(Size);TIME(); cxc.SetSize(Size); TIME("ALLOC");
  TLOOP(10);
  TIME();
  //cxc = cxa OPID cxb;
  FUNC(cxc,cxa,cxb);
  TIME("libsig "+OPNAME);
  ENDTLOOP();

  TX sum = 0;
  for(int ii=0;ii<Size;ii++)sum = sum + cxc[ii];
  cout<<"check sum heavy: "<<sum<<endl;

  TLOOP(10);
  TIME();
  cxc = cxa OPID cxb;
  //FUNC(cxc,cxa,cxb);
  TIME("libsig wrapper "+OPNAME);
  ENDTLOOP();

  sum = 0;
  for(int ii=0;ii<Size;ii++)sum = sum + cxc[ii];
  cout<<"check sum heavy: "<<sum<<endl;
 }


void eltop_sligth(testclass<TX,VY>& Tc, const testclass<TX,VY>& Ta,  const testclass<TX,VY>& Tb)
{
  int size_a = Ta.GetSize();						 
  int size_b = Tb.GetSize();						 
  int size_c = Tc.GetSize();						 
  if(size_c == 0)							 
    {									 
      Tc.SetSize(size_a);						 
      size_c = size_a;						 
    }									 
  if(size_a!=size_b||size_a!=size_c)					 
    {									 
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;		 
      myabort();							 
    };	
  
  const int ElementsPerVector = sizeof(VY) / sizeof(TX);	 
  int nb_iter   = size_a/ElementsPerVector;				 
  int nb_entier = nb_iter*ElementsPerVector;
 
  TX* adra = Ta.bufferA; 
  TX* adrb = Tb.bufferA;
  TX* adrc = Tc.bufferA;
  int ii = 0;
  for(ii = 0;ii<nb_entier;ii += ElementsPerVector){			 
    //Vect(Tc,VY,ii) =  Vect(Ta,VY,ii) +  Vect(Tb,VY,ii);	
    //*((VY*)(Tc.bufferA+ii)) = *((VY*)(Ta.bufferA+ii)) +  *((VY*)(Tb.bufferA+ii)) ;	 
    *((VY*)(adrc+ii)) = *((VY*)(adra+ii)) OPID  *((VY*)(adrb+ii)) ;
  }


  //VY* adra = (VY*)Ta.bufferA; 
  //VY* adrb = (VY*)Tb.bufferA;
  //VY* adrc = (VY*)Tc.bufferA;
  //int nb_entier_4;nb_entier_4 = nb_entier/4;
  //for(ii = 0;ii<nb_entier_4;ii++)
  //  *((adrc++)) = *((adra++)) +  *((adrb++)) ;

  for(ii = nb_entier;ii<size_a;ii++ )	
    Tc.bufferA[ii] = Ta.bufferA[ii] OPID Tc.bufferA[ii];	

}
void eltop_sligth_precalcul(testclass<TX,VY>& Tc, const testclass<TX,VY>& Ta,  const testclass<TX,VY>& Tb)
{
  int size_a    = Size;
  int nb_entier = Size;
  int ElementsPerVector = 4;

  TX* adra = Ta.bufferA; 
  TX* adrb = Tb.bufferA;
  TX* adrc = Tc.bufferA;
  int ii = 0;
  for(ii = 0;ii<nb_entier;ii += ElementsPerVector){			 
    *((VY*)(adrc+ii)) = *((VY*)(adra+ii)) OPID  *((VY*)(adrb+ii)) ;
  }

  for(ii = nb_entier;ii<size_a;ii++ )	
    Tc.bufferA[ii] = Ta.bufferA[ii] OPID Tc.bufferA[ii];	

}
void test_sig_sligth()	
{
  testclass<TX,VY> Ta,Tb,Tc;
  Ta.ones(Size);Tb.ones(Size);Tc.SetSize(Size);
  
  /*******************************************/
  TLOOP(10); TIME();
  eltop_sligth_precalcul(Tc,Ta,Tb);
  TIME("Slight precalcul"+OPNAME); ENDTLOOP();

  TX sum = 0;
  for(int ii=0;ii<Size;ii++)sum = sum + Tc.bufferA[ii];
  cout<<"check sum sligth: "<<sum<<endl;

  TLOOP(10); TIME();
  eltop_sligth(Tc,Ta,Tb);
  TIME("Slight "+OPNAME); ENDTLOOP();

  sum = 0;
  for(int ii=0;ii<Size;ii++)sum = sum + Tc.bufferA[ii];
  cout<<"check sum sligth: "<<sum<<endl;
  //cout<<"check res: "<<Tc.val(111)<<endl;
}


#include "benchTime.h"
void test_time_ms(){

    BenchTime T1;
    BenchTime T2;
    testclass<TX,VY> Toto;Toto.ones(Size);
   // //char* toto = new char[16];
   //  //char* pMEM= new char[8192*10];// __attribute__((  aligned(16)  ));
    char pMEM[8192*10] __attribute__((  aligned(64)  ));
    sta::MEM = pMEM;
   //  //sta::MEM = Toto.tab; //sta::init();
   //  char pa[8192]  __attribute__((  aligned(16)  ));
   //  char pb[8192]  __attribute__((  aligned(16)  ));
   //  char pc[8192]  __attribute__((  aligned(16)  ));;
    AlignedArray<TX,VY> cxa,cxb, cxtmp;
    //    AlignedArray<TX,VY> cxato,cxbto, cxtmpto;

    //cxa.ones(Size);cxb.ones(Size);cxtmp.SetSize(Size);
    cxa.SetSize(Size)  ; //cxato.SetSize(Size);
    cxb.SetSize(Size)  ; //cxbto.SetSize(Size);
    cxtmp.SetSize(Size); //cxtmpto.SetSize(Size);
    
    // int64 diff1 =  (int64)cxa.bufferA-(int64)pMEM;
    // int64 diff2 =  (int64)cxb.bufferA-(int64)pMEM;diff2 = (diff2/8192)*8192;
    // int64 diff3 =  (int64)cxtmp.bufferA-(int64)pMEM;diff3 = (diff3/8192)*8192;

    // DUMP(diff1); DUMP(diff2); DUMP(diff3); mygetchar();
    // // //cxa.bufferA = (float*)pa; cxb.bufferA = (float*)pb; cxtmp.bufferA = (float*)pc;
    int64 taddr; taddr = (int64)pMEM;
    // cxa.bufferA = (float*)(taddr+diff1);//pMEM;
    // cxb.bufferA = (float*)(taddr+diff2);//(pMEM+8192); 
    // cxtmp.bufferA = (float*)(taddr+diff3);//(pMEM+8192*2);
    cxa.bufferA = (float*)(taddr);//pMEM;
    cxb.bufferA = (float*)(taddr+8192);//(pMEM+8192); 
    cxtmp.bufferA = (float*)(taddr+8192*2+16);//(pMEM+8192*2);
    for(int ii=0;ii<Size;ii++){cxa[ii] = 1;cxb[ii] = 1;}
    //for(int ii=0;ii<Size;ii++){cxtmp.val(ii) = cxa.val(ii)+cxb.val(ii);}


    testclass<TX,VY> Ta,Tb,Tc;

    Ta.ones(Size);Tb.ones(Size);Tc.SetSize(Size);
    // Ta.SetSize(Size);Tb.SetSize(Size);Tc.SetSize(Size);
    // Ta.bufferA = (float*)pa; Tb.bufferA = (float*)pb; Tc.bufferA = (float*)pc;
    // for(int ii=0;ii<Size;ii++){Ta.val(ii) = 1;Tb.val(ii) = 1;}

 
    int nbIter = 500000;

    //-------------------------------------------------//

    T2.start();
    TLOOP(nbIter);
    //add_sse_3(a,b,c, Size); 
    //add_sse_3(cxa.bufferA,cxb.bufferA,cxtmp.bufferA, Size); 
    //cxtmp = cxa OPID cxb;
    FUNC(cxtmp,cxa,cxb);
    //cxtmp = cxa;
    ENDTLOOP();
    T2.stop();

    T1.start();
    TLOOP(nbIter);
    eltop_sligth_precalcul(Tc,Ta,Tb);
    //eltop_sligth(Tc,Ta,Tb);
    ENDTLOOP();
    T1.stop();
    
    printf("\nFast %s operation: ( c[i] =a[i] + b[i]  for_all_i )\n"
            " dim   = %i\n"
            " nIter = %i\n"
            " Benchmark Timings (all times in milliseconds):\n"
	   ,OPNAME.c_str(),Size,nbIter); 
    T1.print("  Sligth Compute");
    T2.print("  Heavy  Compute");

    TX sum_sligth = 0;
    for(int ii=0;ii<Size;ii++)sum_sligth = sum_sligth + Tc.bufferA[ii];
    cout<<"check sum_sligth: "<<sum_sligth<<endl;

    TX sum_heavy = 0;
    for(int ii=0;ii<Size;ii++)sum_heavy = sum_heavy + cxtmp[ii];
    cout<<"check sum_heavy: "<<sum_heavy<<endl;
}

int main(){

  OVERHEAD();
  //test_sig_overhead();
  //test_sig_sligth();
  test_time_ms();

  return 1;
}
