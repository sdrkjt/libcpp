#include "class_example.hh"
#include "TSC.hh"

void module::init(int size){
  Sa.SetSize(size);//Sas.SetSize(size);
  Sb.SetSize(size);//Sbs.SetSize(size);
  for(int kk=0;kk<size;kk++)
    {
      Sa.val(kk) = 0.123f + kk;
      Sb.val(kk) = 0.789f + kk;
    }
 
  for (int ii=0;ii<nbLu;ii++ ){
    Sc[ii].zeros(size);
  }
}

void module::compute(){

  TIME();
  Sc[0] = 12;
  TIME("Sc[0] = 12");
  
  TIME();
  Sc[4] = Sa + Sb;
  TIME("ADD");
  
  Sc[5] = Sa - Sb;
  TIME("SUB");
  Sc[6] = Sa * Sb;
  TIME("MUL");
  Sc[7] = Sa / Sb;
  TIME("DIV");

  Sc[8] = sqrt(Sb);
  TIME("SQRT");

  /******test operator with scalar***********/
  Sc[9] = Sa + 2.0f;
  TIME("ADD_scalar");
    
  Sc[10]=  1.0f * Sb ;
  TIME("MUL_scalar");
  /*******test multiple operators************/
  Sc[11] = Sa + Sb * 3.0f;
  TIME("Sc_11 = Sa + Sb * 3.0f");

  Sc[12] = Sb * 2.0f + 1000.0f ;
  TIME("Sc_12 =  Sb * 2.0f + 1000.0f");

  Sc[13] = Sa + Sb*Sc[12];
  TIME("Sc_13 = Sa + Sb*Sc_12");
  
  TIME();
  Sc[4] = rsqrt(Sb);
  TIME("RSQRT Sb");
  
  DUMP(Sb);DUMP(Sc[4]);
   
  TIME();
  Sc[4] = inv(Sa);
  TIME("INV Sa");
  
  DUMP(Sc[4]);
  
  TIME();

  Sc[10] = Sa;
  TIME("Copy");
  // mygetchar();

}
