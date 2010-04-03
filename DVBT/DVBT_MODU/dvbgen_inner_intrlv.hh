#ifndef dvbgen_inner_intrlv_hh
#define dvbgen_inner_intrlv_hh
#include "dvbgen_param.hh"
class InnerIntrlv {
public:
  /*!
    P_constel = 2,4,6
    P_Nmax    = 1512, 3024, 6048
    P_Mode    = Mode2K,Mode4K,Mode8K//2048, 4096, 8192
    P_bitintrlv_sz = 126;

    input_sz_byte = P_Nmax*P_constel/8 (378->4536)
    ouput_sz_byte = P_Nmax
   */ 
  int P_constel;
  int P_Nmax;
  int P_Mmax;
  //for bit interleaver
  //ibit on ibyte goes to o_bit on o_sym
  uint8  tab_bit[378][8];//bit index //(126*v*4)/8
  uint16 tab_sym[378][8];//sym index //maxsym = 126*4 
  Sigui8 O_intrlv_bit; 
  Sigui8 O_intrlv_sym;
  //for symbol interleaver
  int symMod2; 
  Sigui32 Hq;
  //for mapper
  int alpha;
  Sigcxi8 tab_map;


#ifdef check_inner_intrlv 
  FILE * DUMP_inner_intrlv_bit;
  FILE * DUMP_inner_intrlv_sym;
  FILE * DUMP_input;
  FILE * DUMP_mapper;
#endif

  InnerIntrlv(DvbParam::Mode P_Mode, DvbParam::Constel vP_constel, int valpha){
    P_Mmax = P_Mode;

    switch(P_Mmax)
      {
      case  2048: P_Nmax = 1512; break;
      case  4096: P_Nmax = 3024; break;
      case  8192: P_Nmax = 6048; break;
      default : MSG_ERROR("unknown Mode!!"); break;
      }

    P_constel = vP_constel;
    alpha     = valpha;

    /**Build lookup table for bit interleaver**/
    int8 iIntrl[6];//for demux
    
    switch(P_constel)
      {
      case  DvbParam::QPSK  : {int8 viIntrl[] = {0, 1,-1,-1,-1,-1};memcpy(iIntrl,viIntrl,6);} break;
      case  DvbParam::QAM16 : {int8 viIntrl[] = {0, 2, 1, 3,-1,-1};memcpy(iIntrl,viIntrl,6);} break;
      case  DvbParam::QAM64 : {int8 viIntrl[] = {0, 2, 4, 1, 3, 5};memcpy(iIntrl,viIntrl,6);} break;
      default : MSG_ERROR("unknown Constel!!"); break;
      }
    int dIntrl[6] = {0,63,21,84,105,42};//interleaver delay = 126-{0,63,105,42,21,84}

    int useful_sz = 63*P_constel; //(126*4*P_constel)/8;
    int bitcount = 0;
    for(int ibyte=0;ibyte<useful_sz;ibyte++){
      for(int ibit=7;ibit>=0;ibit--){ //MSB first
	uint8  o_bit;
	uint16 o_sym;

	int isym       = bitcount/P_constel;
	int imodv      = bitcount  - P_constel*isym;
	int curIntrl   = iIntrl[imodv]; //current Interleaver
	int ibloc      = isym/126;
	int ilocalsym  = isym-ibloc*126;

	o_sym = 126*ibloc + (ilocalsym + dIntrl[curIntrl])%126;
	o_bit = P_constel-curIntrl; // y = ( a_0,a_1,...,a_(v-1) ) 

	//ibit on ibyte goes to o_bit on o_sym
	tab_bit[ibyte][ibit] = o_bit; 
	tab_sym[ibyte][ibit] = o_sym;

	bitcount++;
      }
    }

    O_intrlv_bit.zeros(P_Nmax); SIGNAME(O_intrlv_bit);

    /***Build lookup table Hq for Symbol Interleaver****/
    int Nr = (P_Mmax==2048) ? 11 : 13;
    Hq.zeros(P_Nmax); 
    uint8 perm_table[12];

    switch (P_Mmax)
      {
      case 2048: { uint8 vperm_table[]={4, 3, 9, 6, 2, 8, 1, 5 , 7, 0}       ; memcpy(perm_table,vperm_table,10); } break;
      case 8192: { uint8 vperm_table[]={7, 1, 4, 2, 9, 6, 8, 10, 0, 3, 11, 5}; memcpy(perm_table,vperm_table,12); } break;
      default:  MSG_ERROR("unsupported OFDM Mode!!"); break;
      }

    Sigui8 Rpi; Rpi.SetSize(Nr-1);
    Sigui8 Ri; Ri.SetSize(Nr-1); 

    int qq = 0;
    for(int ii = 0;ii<P_Mmax;ii++){
      /* compute Ri */
      if(ii<2 ) { 
	Rpi = 0;
      }
      else if(ii==2){
	Rpi = 0; Rpi[0] = 1;    
      }
      else {
	Sigui8 Rpi_old; Rpi_old = Rpi;
	Rpi(0,Nr-3) = Rpi_old(1,Nr-2) ;
	if(P_Mmax==2048){
	  Rpi[Nr-2] = Rpi_old[0] ^ Rpi_old[3];
	}
	else if(P_Mmax==8192) {
	  Rpi[Nr-2] = Rpi_old[0] ^ Rpi_old[1] ^ Rpi_old[4] ^ Rpi_old[6];
	}
	else myabort();
      } 

      for (int kk = 0; kk<Nr-1;kk++){
	Ri[perm_table[kk]] = Rpi[kk];
      }

      /* compute H(q) */
      Hq[qq] = (ii&1) * (1<<(Nr-1));
      for (int jj = 0;jj<Nr-1;jj++){
      	Hq[qq] +=  Ri[jj] * (1<<jj);
      }
      if (Hq[qq] < P_Nmax){
      	qq = qq + 1; 
      }

    }

    O_intrlv_sym.SetSize(P_Nmax); SIGNAME(O_intrlv_sym);

    /***Hq is built for Symbol Interleaver*****************/
    
    symMod2 = 0; /*init symbol counter for interleaver*/

    /***Build lookup table for mapper**********************/
    int nb_point = pow(2,P_constel);    
    tab_map.SetSize(nb_point);
  
    for(int ipoint=0; ipoint<nb_point; ipoint++){
      int re_z,im_z;    
      for(int jj=0;jj<P_constel;jj+=2){
	int bit0 = GETBIT(ipoint,P_constel-jj);
	int bit1 = GETBIT(ipoint,P_constel-jj-1);
	if(jj == 0){
	  re_z = 1 - 2*bit0 ;	    
	  im_z = 1 - 2*bit1 ;
	}
	else{
	  re_z = re_z *(3 - 2*bit0) ;	    
	  im_z = im_z *(3 - 2*bit1) ;
	}
      }
      // add offset for non-uniform mapping
      re_z+=((re_z>0) ? 1:-1) * (alpha-1);
      im_z+=((im_z>0) ? 1:-1) * (alpha-1);
      tab_map[ipoint] = Cxi8(re_z,im_z);
    }

#ifdef check_inner_intrlv
    DUMP_inner_intrlv_bit  = fopen ("z_dump/inner_intrlv_bit.txt", "w");
    DUMP_input             = fopen ("z_dump/inner_intrlv_input.bin", "wb");
    DUMP_inner_intrlv_sym  = fopen ("z_dump/inner_intrlv_sym.txt", "w");
    DUMP_mapper            = fopen ("z_dump/mapper.txt", "w");
#endif

  }
    
  ~InnerIntrlv(){
#ifdef check_inner_intrlv
    fclose(DUMP_inner_intrlv_bit ) ; MSG("Write File: z_dump/inner_intrlv_bit.txt");
    fclose(DUMP_input        )     ; MSG("Write File: z_dump/inner_intrlv_input.bin");
    fclose(DUMP_inner_intrlv_sym)  ; MSG("Write File: z_dump/inner_intrlv_sym.txt");
    fclose(DUMP_mapper          )  ; MSG("Write File: z_dump/mapper.txt");
#endif
  }

  int get_input_sz(){
    return    P_Nmax*P_constel/8;
  }
  int get_output_sz(){
    return   P_Nmax*2;//complex16bits: 8bits real, 8bits image
  }

  void compute(Sigui8 &O_data, Sigui8 &I_data)
  { 

// #ifdef check_inner_intrlv
//     for(int ii=0;ii<I_data.size();ii++){
//       fwrite(I_data.bufferA+ii , 1 , 1 , DUMP_input );
//     }
// #endif

#ifdef BOUNDSCHECKING
    if(O_data.size()!=get_output_sz())MSG_ERROR("output size is not correct");
    if(I_data.size()!=get_input_sz())MSG_ERROR("input size is not correct");
#endif

    /************ Bit interleaver *******************************************************/
    int nb_bbloc    = P_Nmax/(126*4);//1 bbloc = 4 blocs
    int bbloc_sz_i  = 63* P_constel ;//unit = byte
    int bbloc_sz_o  = 126*4         ;//unit = symbol

    int ibyte = 0;

    int pidata = 0;
    int podata = 0;

    for(int ibbloc=0;ibbloc<nb_bbloc;ibbloc++){
      for(int ibyte=0;ibyte<bbloc_sz_i;ibyte++){
	uint8 input = I_data[pidata++];
	SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][0]],tab_bit[ibyte][0],GETBIT(input,1));
	SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][1]],tab_bit[ibyte][1],GETBIT(input,2));
	SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][2]],tab_bit[ibyte][2],GETBIT(input,3));
	SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][3]],tab_bit[ibyte][3],GETBIT(input,4));
	SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][4]],tab_bit[ibyte][4],GETBIT(input,5));
	SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][5]],tab_bit[ibyte][5],GETBIT(input,6));
	SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][6]],tab_bit[ibyte][6],GETBIT(input,7));
        SETBIT(O_intrlv_bit[podata+tab_sym[ibyte][7]],tab_bit[ibyte][7],GETBIT(input,8));
      }
      podata += bbloc_sz_o;
    }
    /************ End Bit interleaver ***************************************************/
 
#ifdef check_inner_intrlv
    // for(int ii=0;ii<P_Nmax;ii++)
    //   fprintf (DUMP_inner_intrlv_bit, "%u \n",GETBIT(O_intrlv_bit[ii],4));
    // for(int ii=0;ii<P_Nmax;ii++)
    //   fprintf (DUMP_inner_intrlv_bit, "%u \n",GETBIT(O_intrlv_bit[ii],3));
    // for(int ii=0;ii<P_Nmax;ii++)
    //   fprintf (DUMP_inner_intrlv_bit, "%u \n",GETBIT(O_intrlv_bit[ii],2));
    // for(int ii=0;ii<P_Nmax;ii++)
    //   fprintf (DUMP_inner_intrlv_bit, "%u \n",GETBIT(O_intrlv_bit[ii],1));
    for(int ii=0;ii<P_Nmax;ii++)
      fprintf (DUMP_inner_intrlv_bit, "%u \n",O_intrlv_bit[ii]);
#endif
    /************ Symbol Interleaver*****************************************************/
    if (symMod2==0){
      for(int ii=0;ii<P_Nmax;ii++){
	O_intrlv_sym[Hq[ii]] = O_intrlv_bit[ii];
      }
     }
    else {
      for(int ii=0;ii<P_Nmax;ii++){
	O_intrlv_sym[ii] = O_intrlv_bit[Hq[ii]]; 
      }
    }
 
    /*update counter*/
    symMod2 = (symMod2+1)&1;

#ifdef check_inner_intrlv
    for(int ii=0;ii<P_Nmax;ii++)
      fprintf (DUMP_inner_intrlv_sym, "%u \n",GETBIT(O_intrlv_sym[ii],4));
    for(int ii=0;ii<P_Nmax;ii++)
      fprintf (DUMP_inner_intrlv_sym, "%u \n",GETBIT(O_intrlv_sym[ii],3));
    for(int ii=0;ii<P_Nmax;ii++)
      fprintf (DUMP_inner_intrlv_sym, "%u \n",GETBIT(O_intrlv_sym[ii],2));
    for(int ii=0;ii<P_Nmax;ii++)
      fprintf (DUMP_inner_intrlv_sym, "%u \n",GETBIT(O_intrlv_sym[ii],1));
#endif
    /************ Mapper*****************************************************************/
    Sigcxi8 O_datacx = Sigcxi8(O_data); 

    for(int ii=0;ii<P_Nmax;ii++){
      O_datacx[ii] = tab_map[O_intrlv_sym[ii]];
    }
 
#ifdef check_inner_intrlv
    Sigi8 uOut = Sigi8(O_data);
    for(int ii=0;ii<P_Nmax;ii++){
      fprintf (DUMP_mapper, "%f \n",float(uOut[2*ii])/sqrt(10.0));
      fprintf (DUMP_mapper, "%f \n",float(uOut[2*ii+1])/sqrt(10.0));
    }
#endif
  }


};


#endif
