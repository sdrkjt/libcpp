#include "sig_gui.hh"
#include "TSC.hh"
#include "libsig.hh"

#include "class_example.hh"
#include <complex>

// set number of times to run test
#define NUMTESTS  10


const Sigfl& test_copy(Sigfl& sa, const Sigfl& sb){

  cout<<"HELLO WORLD\n";
  return sa;
}

#define Size 8192
#define Size_4 Size/4

float ClockTest (__int64 clocks[]) {

  OVERHEAD();

  // Make instance of AlignedArray containing objects of type float:
  Sigfl Sc, Sa, Sb;
  Sigi16 Scs,Sas,Sbs;
  Sigi8 Sc_char,Sa_char,Sb_char;

  // Set the number of entries in the array. All are initialized to 0.
  // Make number divisible by the number of elements in a vector (4)

  float A [Size];
  float B [Size];
  float C [Size];  float C_1 [Size];  float C_2 [Size];

  Sa.SetSize(Size);Sb.SetSize(Size);
  Sas.SetSize(Size);Sbs.SetSize(Size);
  Sa_char.SetSize(Size);Sb_char.SetSize(Size);

//   TIME();
//   Sc.SetSize(Size);
//   TIME("Alloc Sigfl");
  Scs.SetSize(Size);
  TIME("Alloc Sigsh");
  Sc_char.SetSize(Size);
  TIME("Alloc Sigch");

  // Put data into the array by index
  for(int kk=0;kk<Size;kk++)
    {
      A[kk] = 0.123f + kk; 
      Sa[kk] = 0.123f + kk;

      B[kk] = 0.789f + kk;
      Sb[kk] = 0.789f + kk;

      Sas[kk] = (    kk)%4096;
      Sbs[kk] = (9 + kk)%4096;

      Sa_char[kk] = (    kk)%120;
      Sb_char[kk] = (9 + kk)%120;

      //Sc[kk] = 0;
    }

  // loop to measure clock cycles of test code
  for (int i=0; i < NUMTESTS; i++) {
    __int64 before = ReadTSC();                   // clock count before
   
    // ###################### Test code here ######################
#if 1
    TIME();
    /******test operator with with two args****/
    for(int jj=0;jj<Size;jj ++){
      C[jj  ] = A[jj  ] +  B[jj  ];
    }
    TIME("ADD_normal");
        
    for(int jj=0;jj<Size;jj += 4){
      C_1[jj  ] = A[jj  ] +  B[jj  ];
      C_1[jj+1] = A[jj+1] +  B[jj+1];
      C_1[jj+2] = A[jj+2] +  B[jj+2];
      C_1[jj+3] = A[jj+3] +  B[jj+3];
    }
    TIME("ADD_normal_unroll");
    
    Sigfl Sc_4; Sc_4 = Sa + Sb;
    TIME("ADD");
    TIME();
    Sigfl tmp = Sc_4(0,8191); 
    TIME("SUB SIG");
    cout<<tmp<<endl;
    TIME();
    Sigfl Sc_5; Sc_5 = Sa - Sb;
    TIME("SUB");
    Sigfl Sc_6; Sc_6 = Sa * Sb;
    TIME("MUL");
    Sigfl Sc_7; Sc_7 = Sa / Sb;
    TIME("DIV");

    Sigi16 Scs_1; Scs_1 = Sas+Sbs;
    TIME("ADD_Short");

    Sigi8 Sc_char_1; Sc_char_1 = Sa_char + Sb_char;
    TIME("ADD_char");

    /******test operator with with one arg*****/
    for(int jj=0;jj<Size;jj += 4){
      C_2[jj  ] = sqrt(A[jj]);
      C_2[jj+1] = sqrt(A[jj+1]);
      C_2[jj+2] = sqrt(A[jj+2]);
      C_2[jj+3] = sqrt(A[jj+3]);
    }
    TIME("SQRT_normal");

    Sigfl Sc_8; Sc_8 = sqrt(Sb);
    TIME("SQRT");

    Sigfl S_8_0; S_8_0 = rsqrt(Sa);
    TIME("RSQRT");

    Sigfl S_8_1; S_8_1 = inv(Sb);
    TIME("INV");
    /******test operator with scalar***********/
    Sigfl Sc_9;Sc_9 = Sa + 2.0f;
    TIME("ADD_scalar");

    /*Code Not optimal here*/			
    Sigfl VScalar; VScalar.SetSize(4);		
    for(int ii=0;ii<4;ii++)VScalar[ii] = 2.0f;	
    /*---------------------*/				
    TIME("Code Not optimal");
    
    Sigfl Sc_10; Sc_10=  1.0f * Sb ;
    TIME("MUL_scalar");
    /*******test multiple operators************/
    Sigfl Sc_11; Sc_11 = Sa + Sb * 3.0f;
    TIME("Sc_11 = Sa + Sb * 3.0f");

    Sigfl Sc_12; Sc_12 = Sb * 2.0f + 1000.0f ;
    TIME("Sc_12 =  Sb * 2.0f + 1000.0f");

    Sigfl Sc_121; Sc_121 = Sa + Sb*Sc_12;
    TIME("Sc_121 = Sa + Sb*Sc_12");
    /********test methods to init sig**********/
    Sigfl S_13; S_13.ones(Size);
    TIME("ONES Init");

    Sigfl S_14; S_14.zeros(Size);
    TIME("ZEROS Init");

#endif
    TIME();
    for(int jj=0;jj<Size;jj += 4){
      C_1[jj  ] = A[jj  ] +  B[jj  ];
      C_1[jj+1] = A[jj+1] +  B[jj+1];
      C_1[jj+2] = A[jj+2] +  B[jj+2];
      C_1[jj+3] = A[jj+3] +  B[jj+3];
    }
    TIME("ADD_normal_unroll");
    for(int jj=0;jj<Size;jj += 4){
      C_1[jj  ] = A[jj  ] +  B[jj  ];
      C_1[jj+1] = A[jj+1] +  B[jj+1];
      C_1[jj+2] = A[jj+2] +  B[jj+2];
      C_1[jj+3] = A[jj+3] +  B[jj+3];
    }
    TIME("ADD_normal_unroll");

    TIME();
    Sc = Sa+Sb;
    TIME("ADD");

    TIME();
    Sc = Sa+Sb;
    TIME("ADD");
   
    /* Inline assembly example:
       Use this code if you need inline assembly with the Gnu compiler:

       __asm__ __volatile__ (
       " .intel_syntax noprefix \n"  // Switch to Intel syntax
       " .rept 100              \n"  // Repeat if needed

       " shr eax,1              \n"  // Instruction to test

       " .endr                  \n"  // End of repeat
       " .att_syntax prefix     \n"  // Switch back to AT&T syntax
       );
    */
      
    // ###################### Test code end  ######################

    clocks[i] = ReadTSC() - before;       // clock count after
  }

  // subtract overhead from clock counts
  for (int i=0; i < NUMTESTS; i++) {
    clocks[i] -= speed::overhead;
  }
 
  DUMP(Sa); DUMP(Sb);DUMP(Sc);
  DUMP(Sas); DUMP(Sbs);DUMP(Scs);
  DUMP(Sa_char); DUMP(Sb_char);DUMP(Sc_char);
  float sum = 0;
  for(int i=0;i<Size;i++)
    sum += C[i]+C_1[i]+C_2[i];

  return sum;//for testing OPTION -O3
}

void test_complexd(){
  MSGc("Start******TEST complexd",c_red);
  /**INIT**********************************************************/
  complex<double> fa [Size];
  complex<double> fb [Size];
  complex<double> fc [Size];
  for(int ii=0;ii<Size;ii++)
    {
      fa[ii] = ii*0.01;      fb[ii] = ii*0.0123;
    }


  // Example of use:
  // Second order complex polynomial:
  Cxd x(0.131,9.313); DUMP(x);
  const Cxd a(1.,2.), b(3.,4.), c(5.,6.);
  Cxd res =  (a * x + b) * x + c;
  DUMP(res);

  Sigcxd cxa,cxb,cxc;

  cxa.SetSize(Size);cxb.SetSize(Size);cxc.SetSize(Size);
  //cxa.SigStep(0,1,Size);
  for(int kk=0;kk<Size;kk++)
    {
      cxa[kk] = _mm_set_pd(kk*0.1,kk*0.7);
      cxb[kk] = _mm_set_pd(kk*0.11,kk*0.27);
    }
 /**FIN INIT**********************************************************/
  for(int iter=0;iter<3;iter++){
    TIME();
    for(int ii=0;ii<Size;ii++)
      fc[ii] = fa[ii]+fb[ii];
    TIME("ADD Normal");
    TIME();
    for(int ii=0;ii<Size;ii++)
      fc[ii] = fa[ii] * fb[ii];
    TIME("MUL Normal");
    TIME();
    for(int ii=0;ii<Size;ii++)
      fc[ii] = fa[ii] / fb[ii];
    TIME("DIV Normal");
  }

  for(int iter=0;iter<3;iter++){
    /**CODE**********************************************************/
    TIME();
    cxc = cxa + cxb;
    TIME("ADD Complexd");

    TIME();
    cxc = cxa * cxb;
    TIME("MUL Complexd");

    DUMP(cxa);  DUMP(cxb);
    DUMP(cxc);

    TIME();
    cxc = cxa / cxb;
    TIME("DIV Complexd");

    TIME();
    cxc = -cxa;
    TIME("OPPOSITE cxa");
    DUMP(cxc);

    TIME();
    cxc = conj(cxa);
    TIME("CONJ cxa");
    DUMP(cxc);

    TIME();
    cxc = abs2(cxb);
    TIME("abs2(cxb)");
    DUMP(cxc);

    TIME();
    cxb = normcx(cxc);
    TIME("NORMCX: cxb = normcx(cxc)");
    DUMP(cxb);
    /**FIN CODE**********************************************************/
  }
  MSGc("End******TEST complexd",c_red);
}

int test_complexs(int argc, char **argv){

  MSGc("Start******TEST complexs",c_red);
  // Example of use:
  // Second order complex polynomial:
  Cxs x(0.131f,9.313f); DUMP(x);
  const Cxs a(1.f,2.f), b(3.f,4.f), c(5.f,6.f);
  Cxs res =  (a * x + b) * x + c;
  DUMP(res);
  if(0)
    {
      Cxs __attribute__ (( aligned(16) )) vec1[2];
      Cxs __attribute__ (( aligned(16) )) vec2[2];
      Cxs __attribute__ (( aligned(16) )) vec3[2];

      vec1[0]=a; vec1[1] = b;
      vec2[0]=c; vec2[1] = x;
      __v2Cxs vecCx_1 = *(__v2Cxs*)vec1;
      __v2Cxs vecCx_2 = *(__v2Cxs*)vec2;
      cout<<vec1[0]<<" "<<vec1[1]<<endl;
      float debug[4];
      _mm_store_ps (debug,vecCx_1);
      cout<<debug[0]<<" "<<debug[1]<<" "<<debug[2]<<" "<<debug[3];mygetchar();
  
      cout<<vec2[0]<<" "<<vec2[1]<<endl;
      *(__v2Cxs*)vec3 = vecCx_1 * vecCx_2;
      cout<<vec3[0]<<" "<<vec3[1]<<endl;
      cout<<a*c<<" "<<b*x<<endl;
    }

  Sigcxs cxa,cxb,cxc;

  cxa.SetSize(Size);cxb.SetSize(Size);cxc.SetSize(Size);
  //cxa.SigStep(0,1,Size);
  for(int kk=0;kk<Size;kk++)
    {
      cxa[kk] = Cxs(kk*0.7,kk*0.1);
      cxb[kk] = Cxs(kk*0.27,kk*0.11);
    }
  figure(1);plot2d(cxa,"cxa");
  figure(2);plot2d(cxb,"cxb");
  DUMP(cxa);  DUMP(cxb);
  cxc = cxa + cxb;
  TIME();
  cxc = cxa + cxb;
  TIME("ADD Complexs");
  TIME();
  cxc = cxa * cxb;
  TIME("MUL Complexs: cxc = cxa * cxb");
  TIME();
  cxc = cxa / cxb;
  TIME("DIV Complexs");

  //DUMP(cxa);  DUMP(cxb);
  //cxa = cxc * cxb;
  //DUMP(cxa);

  TIME();
  cxc = conj(cxb);
  TIME("conj(cxb)");
  DUMP(cxc);
  TIME();
  cxc = cxb*conj(cxb);
  TIME("cxb*conj(cxb)");
  DUMP(cxc);

  TIME();
  cxc = abs2(cxb);
  TIME("abs2(cxb)");
  DUMP(cxc);
  //figure(3);plot2d(cxc,"cxc");
  TIME();
  cxc = -cxa;
  TIME("-cxa");
  DUMP(cxc);

  TIME();
  cxb = normcx(cxc);
  TIME("NORMCX: cxb = normcx(cxc)");
  DUMP(cxb);
  cxb[0]=1;
  //figure(4);plot2d(cxb,"cxb");
  TIME();
  cxc = cxb*Cxs(2.f);
  TIME("cxc = 2*cxb");
  DUMP(cxc)
    ;

  MSGc("End******TEST complexs",c_red);
  return -1;
}
void test_example(){
  module Mod;
  Mod.init(8192);
  
  int nbiter = 10;
  for(int ii=0;ii<nbiter;ii++)
    {    Mod.compute(); cout<<"xxxxxxxx\n"; }
}
void test_clocktest(){
   // list of clock counts
   __int64 clocklist[NUMTESTS];
   int i;

   // run tests
   ClockTest (clocklist);

   // print results
   printf ("\n  test     clock cycles");
   for (i = 0; i < NUMTESTS; i++) {
     printf ("\n%6i  %14G", i+1, double(clocklist[i]));
   }

   printf ("\n  MEAN     clock cycles");
   __int64 mean = clocklist[0];
   for (i = 1; i < NUMTESTS; i++) {
     mean += clocklist[i];
   }
   mean /= NUMTESTS;

   printf ("\n%6i  %14G", 00, double(mean));
   printf ("\n");
}
int test_random(int argc, char **argv){
#ifdef ASM
 int seed = 98765432;                          
 MersenneRandomInit(seed);
#endif

  Sigfl A;

  TIME();
  A.random(Size);
  TIME("random");

  figure(1); plot1d(A,"Random Test");

  return -1;
}

void test_memcpy()
{
  //float* dest1 = new float[Size];
  //float* src1  = new float[Size];
  //float* dest2 = new float[Size];
  //float* src2  = new float[Size];
  
  Sigfl sigdst,sigsrc;
  sigdst.SetSize(Size);  sigsrc.SetSize(Size);
   float dest1  [Size];
   float src1   [Size];
   float dest2  [Size];
   float src2   [Size];
  
  float val = 0;
  for(int ii=0;ii<Size;ii++){val+=0.1f; src1[ii]=val;src2[ii]=val;sigsrc[ii]=val;}
 
  for(int ii=0;ii<4;ii++)
    {
      TIME();
      memcpy(dest1, src1, Size);
      TIME("memcpy");
  
#ifdef ASM
      TIME();
      A_memcpy(dest2, src2, Size);
      TIME("A_memcpy");
#endif

      TIME();
      sigdst = sigsrc;
      TIME("Copy Sig");

      TIME();
      for(int ii=0;ii<Size;ii++)
	dest1[ii] = src1[ii];
      TIME("Normal Copy");
    }

  float sum = 0;
  for(int ii=0;ii<Size;ii++){
    sum += dest2[ii]+dest1[ii];
  }
  cout<<sum<<endl;
}
void test_blas(){
 MSGc("Start******TEST BLAS",c_red);

 Sigcxs cxa,cxb,cxc;

 cxa.SetSize(Size);cxb.SetSize(Size);cxc.SetSize(Size);
 //cxa.SigStep(0,1,Size);
 for(int kk=0;kk<Size;kk++)
   {
     cxa[kk] = Cxs(0.7 ,0.1);
     cxb[kk] = Cxs(0.27,0.11);
   }
 TIME();
 float mean = meanAbs2(cxa);
 mean = mean + meanAbs2(cxb);
 TIME();


};
int main(int argc, char **argv)
{
  QApplication a(argc, argv);
  SdrGui   pGui;

  int (*worker) (int, char **);
  MSG("TEST SIZE "<<Size);
  //OVERHEAD();
  //test_clocktest();
  //test_example();
  //test_complexd();
  
  //test_memcpy();
  //test_complexs();
  
  test_blas();

  if(0){ 
    worker = 
      test_complexs;
    //&test_random;
    //NULL;
    
    SdrThread plot(&pGui,worker);
    plot.start();
  }
  return a.exec();
}
