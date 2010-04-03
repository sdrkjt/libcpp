#include "dvbgen_ofdm.hh"


#include "dvbgen_param.hh"
#include "dvbgen_tps.hh"

extern DvbParam DVBT;

OfdmWaveForm::OfdmWaveForm(){
  Lu    = DVBT.Lu;
  Ku    = DVBT.Ku;
  Nmax  = DVBT.Nmax ;
  NbTps = DVBT.NbTps;
  Ls    = Lu + Lu/DVBT.guard;

  /*mask generation*/
  mask_cp  = get_cp_mask();
  mask_tps = get_tps_mask();
  
  Sigui8 Sprbs =  get_prbs();//maxsize 6817
  cx_prbs.SetSize(Ku);
  for(int kk=0;kk<Ku;kk++){
    cx_prbs[kk] = Cxs(4.0/3.0*2.0*(0.5-Sprbs[kk]),0) ;
  }

  /*TPS init*/
  tps_initmap.SetSize(NbTps);
  int itps = 0;
  for(int kk=0;kk<Ku;kk++){
    if(mask_tps[kk]){
      tps_initmap[itps] = Cxs(2.0*(0.5- Sprbs[kk]),0);
      itps++;
    }
  }; 
  sigst_tpsmap.SetSize(NbTps);
  sigst_tpsmap = tps_initmap;

  //normalization factor
  normFact = get_norm_fact();  // data power normalization

  SCarriers.zeros(Ku);
  SFreq.zeros(Lu);
  //STime.zeros(Ls);

  /*start counter*/
  isym = 0;  symCounter = 0;
  ifrm = 0;  iromtps    = 0;

#ifdef check_ofdm
    DUMP_ofdm  = fopen ("z_dump/ofdm.txt", "w");
#endif

}

OfdmWaveForm::~OfdmWaveForm(){
#ifdef check_ofdm
  fclose(DUMP_ofdm          )  ; MSG("Write File: z_dump/ofdm.txt");
#endif
}

void OfdmWaveForm::compute(Sigui8 &O_data,Sigui8 &I_data){
  Sigi8 Data = Sigi8(I_data);
  Sigcxs STime = Sigcxs(O_data);

  // /*----TPS update-----------*/
  // Sigui8 Stpsbit; Stpsbit.SetSize(68);//new tpsbits val
  // if ( isym==0 ){
  //   get_tps_bits( ifrm , Stpsbit);//update tpsbits val
  //   sigst_tpsmap = tps_initmap; //init tpsmapping
  // }
  
  // if(Stpsbit[isym]){ //detect state change
  //   sigst_tpsmap = -sigst_tpsmap;
  // }
  
  /*----Carriers update-----------*/
  int itps = 0; int idat = 0;
  for(int kk=0;kk<Ku;kk++){
    int is_pilot = (mask_cp[kk])||(((kk-3*isym)%12==0));
    int is_tps   = mask_tps[kk];
    //int is_data  = 1-(is_pilot+is_tps);
    // SCarriers[kk] = (  (float)is_pilot * cx_prbs[kk] +
    // 		       (float)is_tps   * sigst_tpsmap[itps] +
    // 		       (float)is_data  * data );
    if(is_pilot){
      SCarriers[kk] = cx_prbs[kk];
    }
    else if(is_tps){
      // SCarriers[kk] = sigst_tpsmap[itps] ;  
      // itps ++; 
      SCarriers[kk] = romtps[iromtps] ; 
      iromtps++; 
    }
    else{
      SCarriers[kk] = Cxs(Data[(idat<<1)],Data[(idat<<1)+1])*normFact; 
      idat ++;
    }
  }

  /*----Frequency domain-----------*/
  int Ku1 = Ku-1; int half_Ku1 = Ku1>>1;
  SFreq(0,half_Ku1) = SCarriers(half_Ku1,Ku1);
  SFreq(Lu-half_Ku1,Lu-1) = SCarriers(0,half_Ku1-1);
  /*----Time domain----------------*/
  int Lg = Ls-Lu;
  STime(Lg,Ls-1) = idft(SFreq);
  STime(0,Lg-1) =  STime(Ls-Lg,Ls-1);

  /*----Float to Short-------------*/

  /*----power normalization--------*/
  STime = STime * (sqrt(2048)*44.48);
  //TIME("OFDM");


  /*update counters*/
  //isym=0->67; ifrm=0->3
  isym++; symCounter++;
  if(isym==68){
    isym = 0;
    ifrm = (ifrm+1)&3;
    if(ifrm==0) iromtps = 0;
  }

#ifdef GUI
  figure(1,"Data");  curve(1);setpen("rd","o",""); plot2d(SCarriers);
  Sigcxs absFreq;absFreq = abs2(SFreq); Sigfl toto; toto = real(absFreq);
  figure(2,"Frequency domain"); plot1d(toto);
  Sigcxs absTime; absTime = abs2(STime);Sigfl titi; titi = real(absTime);
  figure(3,"Time domain"); plot1d(titi);
#endif

#ifdef check_ofdm
  for(int kk=0;kk<Ku;kk++){
   fprintf (DUMP_ofdm, "%f \n",float(SCarriers[kk].Re()));
   fprintf (DUMP_ofdm, "%f \n",float(SCarriers[kk].Im()));
  }
  // for(int kk=0;kk<Ls;kk++){
  //   fprintf (DUMP_ofdm, "%f \n",float(normFact*STime[kk].Re()));
  //   fprintf (DUMP_ofdm, "%f \n",float(normFact*STime[kk].Im()));
  // }
#endif
}

