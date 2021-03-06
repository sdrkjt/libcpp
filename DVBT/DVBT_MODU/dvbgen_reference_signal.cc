#include "dvbgen_reference_signal.hh"

#include "dvbgen_param.hh"
extern DvbParam DVBT;

Sigui8 get_cp_mask(){
  /* continual pilot */
  int Ku = DVBT.Ku;
  Sigui8 cp_mask; cp_mask.SetSize(Ku);
  int cp_pos [] = { 
    0 , 48 , 54 , 87 , 141 , 156 , 192 , 201 , 255 , 279 ,
    282 , 333 ,	432 , 450  , 483  , 525 , 531 , 618 , 636 ,
    714 , 759 , 765 , 780 ,	804  , 873  , 888 , 918 , 939 ,
    942 ,   969 ,  984 , 1050 , 1101 , 1107 , 1110 , 1137 ,
    1140 , 1146 , 1206 , 1269 , 1323 , 1377 , 1491 , 1683 ,
    1704 , 1752 , 1758 , 1791 , 1845 , 1860 , 1896 , 1905 ,
    1959 , 1983 , 1986 , 2037 , 2136 , 2154 , 2187 , 2229 ,
    2235 , 2322 , 2340 , 2418 , 2463 , 2469 , 2484 , 2508 ,
    2577 , 2592 , 2622 , 2643 , 2646 , 2673 , 2688 , 2754 ,
    2805 , 2811 , 2814 , 2841 , 2844 , 2850 , 2910 , 2973 ,
    3027 , 3081 , 3195 , 3387 , 3408 , 3456 , 3462 , 3495 ,
    3549 , 3564 , 3600 , 3609 , 3663 , 3687 , 3690 , 3741 ,
    3840 , 3858 , 3891 , 3933 , 3939 , 4026 , 4044 , 4122 ,
    4167 , 4173 , 4188 , 4212 , 4281 , 4296 , 4326 , 4347 ,
    4350 , 4377 , 4392 , 4458 , 4509 , 4515 , 4518 , 4545 ,
    4548 , 4554 , 4614 , 4677 , 4731 , 4785 , 4899 , 5091 ,
    5112 , 5160 , 5166 , 5199 , 5253 , 5268 , 5304 , 5313 ,
    5367 , 5391 , 5394 , 5445 , 5544 , 5562 , 5595 , 5637 ,
    5643 , 5730 , 5748 , 5826 , 5871 , 5877 , 5892 , 5916 ,
    5985 , 6000 , 6030 , 6051 , 6054 , 6081 , 6096 , 6162 ,
    6213 , 6219 , 6222 , 6249 , 6252 , 6258 , 6318 , 6381 ,
    6435 , 6489 , 6603 , 6795 , 6816 };

  int ipos = 0;
  for (int k = 0; k < Ku; k++){
    cp_mask[k] = 0;
    if ( k == cp_pos[ipos]){
      cp_mask[k] = 1;
      ipos++;
    }
  }
  return cp_mask;
}

Sigui8 get_tps_mask(){
  /* TPS */
  int Ku = DVBT.Ku;
  Sigui8 tps_mask; tps_mask.SetSize(Ku);
  int tps_pos [] = { 
    34, 50, 209, 346, 413, 569, 595, 688, 790, 901, 1073, 1219, 1262, 1286,
    1469, 1594, 1687, 1738, 1754, 1913, 2050, 2117, 2273, 2299, 2392, 2494,
    2605, 2777, 2923, 2966, 2990, 3173, 3298, 3391, 3442, 3458, 3617, 3754, 
    3821, 3977, 4003, 4096, 4198, 4309, 4481, 4627, 4670, 4694, 4877, 5002,
    5095, 5146, 5162, 5321, 5458, 5525, 5681, 5707, 5800, 5902, 6013, 6185,
    6331, 6374, 6398, 6581, 6706, 6799}; 
 
  int ipos = 0;
  for (int k = 0; k < Ku; k++){
    tps_mask[k] = 0;
    if (k == tps_pos[ipos]){
      tps_mask[k] = 1;
      ipos++;
    }
  }

  return tps_mask;
}

Sigui8 get_prbs(){
  /* prbs sequence */
  int Ku = DVBT.Ku;
  Sigui8 Sprbs; Sprbs.SetSize(Ku);
  Sigui8 genprbs; genprbs.ones(11);

  for (int k = 0; k < Ku ; k++){
    Sprbs[k] = genprbs[10];
    int next = 0x1 & ( genprbs[8] ^ genprbs[10] ); 
    for (int j = 10; j >= 1 ; j--){
      genprbs[j] = genprbs[j-1];
    }
    genprbs[0] = next;
  }
  return Sprbs;
}

void get_tps_bits(int ifr , const Sigui8& Sigtps ) {
  bool tps_cellid      = DVBT.cellid;
  int tps_cellid_val   = DVBT.cellidValue;
  int tps_const        = (0*(DVBT.constel==DvbParam::QPSK )+
			  1*(DVBT.constel==DvbParam::QAM16)+
			  2*(DVBT.constel==DvbParam::QAM64));//0-1-2-3:QPSK, 16-QAM, 64-QAM, Reserved
  int tps_native_intlv = DVBT.native_intlv;//native vs in-depth inner intrlv
  int tps_hierarch     = DVBT.hierarch;//hierarchical vs non hierarchical
  int tps_alpha        = DVBT.alpha;
  int tps_crateH       = DVBT.crateH; //1-2-3-5-7: 1/2;2/3;3/4;5/6;7/8
  int tps_crateL       = DVBT.crateL;
  int tps_guard        = DVBT.guard;  //32,16,8,4
  int tps_trmode       = (0*(DVBT.Lu==2048)+
			  1*(DVBT.Lu==8192)+
			  2*(DVBT.Lu==4096)); //0-1-2-3 : 2k;8k;4k;Reserved

  //DUMP(tps_cellid      );
  //DUMP(tps_cellid_val  );
  //DUMP(tps_const       );
  //DUMP(tps_native_intlv);
  //DUMP(tps_hierarch    );
  //DUMP(tps_alpha       );
  //DUMP(tps_crateH      );
  //DUMP(tps_crateL      );
  //DUMP(tps_guard       );
  //DUMP(tps_trmode      ); 
  //mygetchar();

  int SYNCWORD = 0x35ee;

  //s0
  Sigtps[0] = 0;

  //s1 to s16 syncword
  for (int i=1;i<=16;i++){
    if( (ifr&1)==0 ) //first & third tps block
      Sigtps[i] = (SYNCWORD >> (15-i+1))&1;
    else  //second & fourth tps block
      Sigtps[i] = ((SYNCWORD^0xffff) >> (15-i+1))&1;
  }

  //s17 to s22 length
  int usedbit = 0x17 ;
  if (tps_cellid) usedbit = 0x1F;

  for (int i=17;i<=22;i++){
    Sigtps[i] = ((usedbit)>>(5-i+17))&1 ;
  }

  //s23 to s24 num frame
  Sigtps[23] = (ifr>>1) & 1;
  Sigtps[24] = (ifr & 1);

        
  //s25 s26 constellation
  Sigtps[25] = (tps_const>>1) & 1;
  Sigtps[26] = tps_const & 1;


  //s27 to s29 hierachy / interleaver
  if(tps_native_intlv==1)  Sigtps[27] = 0; 
  else                     Sigtps[27] = 1; 

  if (tps_hierarch==0){
    Sigtps[28] = 0;  Sigtps[29] = 0;  
  }
  else{
    switch(tps_alpha)
      {
      case 1 :  Sigtps[28] = 0;  Sigtps[29] = 1;  break ;
      case 2 :  Sigtps[28] = 1;  Sigtps[29] = 0;  break ;  
      case 4 :  Sigtps[28] = 1;  Sigtps[29] = 1;  break ;
      default: cerr<<"unknown value of alpha\n";myabort();
      }
  }
   
  //s30 to s32 code rate HP
  switch(tps_crateH)
    {
    case 1 :  Sigtps[30] = 0 ;   Sigtps[31] = 0 ;   Sigtps[32] = 0 ; break ;
    case 2 :  Sigtps[30] = 0 ;   Sigtps[31] = 0 ;   Sigtps[32] = 1 ; break ;
    case 3 :  Sigtps[30] = 0 ;   Sigtps[31] = 1 ;   Sigtps[32] = 0 ; break ;
    case 5 :  Sigtps[30] = 0 ;   Sigtps[31] = 1 ;   Sigtps[32] = 1 ; break ;
    case 7 :  Sigtps[30] = 1 ;   Sigtps[31] = 0 ;   Sigtps[32] = 0 ; break ;
    default:  Sigtps[30] = 1 ;   Sigtps[31] = 1 ;   Sigtps[32] = 1 ; break ;
    }

  //s33 to s35 code rate LP
  switch(tps_crateL)
    {
    case 1 :  Sigtps[33] = 0 ;   Sigtps[34] = 0 ;   Sigtps[35] = 0 ; break ;
    case 2 :  Sigtps[33] = 0 ;   Sigtps[34] = 0 ;   Sigtps[35] = 1 ; break ;
    case 3 :  Sigtps[33] = 0 ;   Sigtps[34] = 1 ;   Sigtps[35] = 0 ; break ;
    case 5 :  Sigtps[33] = 0 ;   Sigtps[34] = 1 ;   Sigtps[35] = 1 ; break ;
    case 7 :  Sigtps[33] = 1 ;   Sigtps[34] = 0 ;   Sigtps[35] = 0 ; break ;
    default:  Sigtps[33] = 1 ;   Sigtps[34] = 1 ;   Sigtps[35] = 1 ; break ;
    }

  //s36 to s37 guard interval
  switch(tps_guard)
    {
    case 32 :  Sigtps[36] = 0 ;   Sigtps[37] = 0 ;   break ;
    case 16 :  Sigtps[36] = 0 ;   Sigtps[37] = 1 ;   break ;
    case 8  :  Sigtps[36] = 1 ;   Sigtps[37] = 0 ;   break ;
    case 4  :  Sigtps[36] = 1 ;   Sigtps[37] = 1 ;   break ;
    default :  Sigtps[36] = 1 ;   Sigtps[37] = 1 ;   break ;
    }

  //s38 to s39 transmission mode
  Sigtps[38] = (tps_trmode>>1) & 1;
  Sigtps[39] = tps_trmode & 1;


  //s40 to s47 cellid
  if (!tps_cellid){
    for (int ii=40;ii<=47;ii++) Sigtps[ii] = 0;
  }
  else {
    if( (ifr&1)==1 ){
      for (int ii=40;ii<=47;ii++)Sigtps[ii] = (tps_cellid_val>>(7-(ii-40))) & 1;
    }
    else {
      for (int ii=40;ii<=47;ii++)Sigtps[ii] = (tps_cellid_val>>(15-(ii-40))) & 1;
    }
  }

  //s48 to s51 DVB-H signalization
  for (int ii=48;ii<=53;ii++) Sigtps[ii] = 0;

  //s54 to 67 BCH 
  int parity=0;				
  for(int ii=1; ii<=53 ; ii++){ 
    if (( Sigtps[ii] ^ ((parity>>13)&1))==0)
      parity = parity << 1;
    else
      parity = (parity << 1) ^ (0x4377);	  
  }
  for (int ii=54;ii<=67;ii++) Sigtps[ii] = ((parity>>(13-ii+54))&1) ;
  
}
 

float get_norm_fact(){ 
  int P_const = DVBT.constel;//0-1-2-3:QPSK, 16-QAM, 64-QAM
  int P_alpha = DVBT.alpha;

  float norm_fact = 1;

  if (P_const == DvbParam::QPSK){
    norm_fact = 1/sqrt(2.0) ;
  }
  else if (P_const == DvbParam::QAM16){
    if (P_alpha == 1)      norm_fact = 1/sqrt(10.0) ;
    else if (P_alpha == 2) norm_fact = 1/sqrt(20.0) ;
    else if (P_alpha == 4) norm_fact = 1/sqrt(52.0) ;
  }
  else if (P_const == DvbParam::QAM64){
    if (P_alpha == 1)	   norm_fact = 1/sqrt(42.0) ;
    else if (P_alpha == 2) norm_fact = 1/sqrt(60.0) ;
    else if (P_alpha == 4) norm_fact = 1/sqrt(108.0) ;
  }

  return norm_fact;
}
