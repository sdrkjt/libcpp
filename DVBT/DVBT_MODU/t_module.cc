#include "dvbgen_outer_coder.hh"
#include "dvbgen_inner_coder.hh"
#include "dvbgen_inner_intrlv.hh"
#include "dvbgen_ofdm.hh"
#include "SDRApplication.hh"

void get_mpeg_8pck(Sigui8 &O_mpeg8pck){
  CHECK_INIT_SIG(O_mpeg8pck,8*188);

  /*--DATA Bytes--*/
  O_mpeg8pck.random(8*188);

  /*--SYNC Bytes--*/
  for(int isync = 0;isync<1503;isync+=188)
    O_mpeg8pck[isync] = 0x47;
}
void help(){
  cout << "Usage :   ~/$ dvbmodu -i(S,F) filename  -o filename  [OPTIONS]\n";
  cout << " OPTIONS :\n";
  cout << "   -nfft             <arg> : FFT length        (0=2k 2=4k 1=8k) (default=0)\n";
  cout << "   -guard            <arg> : Guard interval    (0=1/32 1=1/16 2=1/8 3=1/4) (default=0)\n";
  cout << "   -nqam             <arg> : Constellation     (0=QPSK 1=16QAM 2=64QAM) (default=2)\n";
  cout << "   -intlv            <arg> : interleaver length (1=native 0=extended) (default=1)\n";
  cout << "   -hrch             <arg> : Hierarchy (0=non-hierarchal 1=hierarchical HP 2=hierarchical LP) (default=0)\n";
  cout << "   -alpha            <arg> : Constellation unifomity (1=uniform 2 and 4=non-uniform) (default=1)\n";
  cout << "   -code_rate_hp     <arg> : high priority stream Viterbi code rate (1=1/2 2=2/3 3=3/4 5=5/6 7=7/8) (default=2)\n";
  cout << "   -code_rate_lp     <arg> : low priority stream Viterbi code rate (1=1/2 2=2/3 3=3/4 5=5/6 7=7/8) (default=2)\n";
  cout << "   -seed             <arg> : random seed\n";
  exit(-1);
};

DvbParam DVBT;

int main_dvbgen(int argc, char **argv)
{

  if (argc < 5) help();

  if (strcmp(argv[1],"-iS")==1&&strcmp(argv[1],"-iF")==1){cerr<<"Input must be specified with -iF or -iS\n"; help();}
  if (strcmp(argv[3],"-o")==1){cerr<<"Output must be specified with -o\n"; help();}

  char* I_type     = argv[1]; //char* O_type     = argv[3] 
  char* I_filename = argv[2]; char* O_filename = argv[4];

  // for (Int i=5 ; i<argc ; i++) {
  //   if (strcmp(argv[i],"-nfft"            )==0)  nfft           = atoi(argv[i+1]); 
  //   if (strcmp(argv[i],"-guard"           )==0)  guard          = atoi(argv[i+1]); 
  //   if (strcmp(argv[i],"-nqam"            )==0)  nqam           = atoi(argv[i+1]); 
  //   if (strcmp(argv[i],"-intlv"           )==0)  native_intlv   = atoi(argv[i+1]);
  //   if (strcmp(argv[i],"-hrch"            )==0)  {hrch          = (atoi(argv[i+1]) != 0);  select_hp      = (atoi(argv[i+1]) != 2); }
  //   if (strcmp(argv[i],"-alpha"           )==0)  alpha          = atoi(argv[i+1]);
  //   if (strcmp(argv[i],"-code_rate_hp"    )==0)  code_rate_hp   = atoi(argv[i+1]);
  //   if (strcmp(argv[i],"-code_rate_lp"    )==0)  code_rate_lp   = atoi(argv[i+1]);
  // }





  char pMEM[MAX_MEM_SIZE] __attribute__((  aligned(64)  ));
  sta::MEM = pMEM; sta::init();
  //enum {QPSK=2, QAM16=4, QAM64=6};

  int P_crate = DVBT.crateH;
 
  int sz_8pck_ts      = 1504; //size in byte
  int sz_8pck_outer   = 1632; 
  int sz_8pck_inner   = sz_8pck_outer * (P_crate+1)/P_crate;
  int sz_inner_input  = 1260; //bytes 
  int sz_inner_output = 1260*(P_crate+1)/P_crate; //bytes 
  int Nmax = 1512 ;//2K; 6048 8K //nb subcarriers
  //int nqam = QAM64;//4bits, 64
  //int nbit_info_hp = (int) ((float)(Nmax * nqam * P_crate) / ( (float) (P_crate+1))) ;
  //int mapper_alpha = 1;
  /****START INIT*******/
  SDRApplication pApp;


  /****Gen mpeg 8pcks********************/
  //SDRModule *Msc_GenTs = new SDRFuncSource(&get_mpeg_8pck, sz_8pck_ts);
  SDRModule *Msc_GenTs;
  if(strcmp(I_type,"-iS")==1){
    Msc_GenTs = new SDRFileSource(//"/media/DATA/DATA_ACQUISITION/dvbt/alizee_image.ts",
				  //"/media/DATA/DATA_ACQUISITION/dvbt/piano_debout.ts",
				  //"REF/ref/random.ts", 
				  //"/media/DATA/DATA_ACQUISITION/dvbt/Alizee_j_en_ai_marre.ts",
				  //"/media/DATA/DATA_ACQUISITION/dvbt/HD_Alizee_j_ai_pas_vingt_ans.ts",
				  I_filename,
				  sz_8pck_ts,"circul");
  }
  else{
    Msc_GenTs = new SDRFifoSource(I_filename, sz_8pck_ts);
  }
  pApp.add_module(Msc_GenTs); MODULE_NAME(*Msc_GenTs);
  
  /****Scrambler + RS Encoding + Byte Interleaving*************/
  OuterCoder *C_Outer = new OuterCoder();
  SDRModule  *M_Outer = new SDRMethCore<OuterCoder>(C_Outer,
  						    &OuterCoder::compute,
  						    sz_8pck_ts,sz_8pck_outer);
  pApp.add_module(M_Outer); MODULE_NAME(*M_Outer);

  /****Convolutional Encoder*************/
  InnerCoder *C_Inner = new InnerCoder(P_crate);
  SDRModule  *M_Inner = new SDRMethCore<InnerCoder>(C_Inner,
  						    &InnerCoder::compute,
  						    sz_inner_input, sz_inner_output);
  pApp.add_module(M_Inner ); MODULE_NAME(*M_Inner);

  /****Bit&Symbol Interleaving + Mapping**********/
  InnerIntrlv *C_InnerIntrlv = new InnerIntrlv(DVBT.Lu, DVBT.constel, DVBT.alpha);
  SDRModule   *M_InnerIntrlv = new SDRMethCore<InnerIntrlv>(C_InnerIntrlv,
  							    &InnerIntrlv::compute,
  							    C_InnerIntrlv->get_input_sz(),C_InnerIntrlv->get_output_sz() );
  pApp.add_module(M_InnerIntrlv ); MODULE_NAME(*M_InnerIntrlv);

  /****OFDM Waveform********************/

  OfdmWaveForm * C_Ofdm = new OfdmWaveForm();
  SDRModule    * M_Ofdm = new SDRMethCore<OfdmWaveForm>(C_Ofdm,
							&OfdmWaveForm::compute,
							C_Ofdm->get_input_sz(),C_Ofdm->get_output_sz() );
  pApp.add_module(M_Ofdm); MODULE_NAME(*M_Ofdm);


  SDRModule  *Dump2file = new SDRFileSink(//"PATTERN/alizee_image.dvbt",
					  //"PATTERN/Alizee_j_en_ai_marre.dvbt",
					  //"PATTERN/HD_Alizee_j_ai_pas_vingt_ans.dvbt",
					  O_filename,
					  C_Ofdm->get_output_sz() );
  pApp.add_module(Dump2file); MODULE_NAME(*Dump2file);


  SDRModule  *Msk_flush = new SDRNullSink(C_Ofdm->get_output_sz());
  //pApp.add_module(Msk_flush); MODULE_NAME(*Msk_flush);

  pApp.connect(6, Msc_GenTs, M_Outer, M_Inner,M_InnerIntrlv,M_Ofdm,Dump2file/*Msk_flush*/);

  /****END INIT*******/
  int ii = 0; double mean_time = 0;
  //LOOP(ii,20)
  do
    {
      TIME();
      pApp.run();
      TIME("DVB MODULATION");
      DUMP(ii);
      ii++;int alpha = (ii>10000)?10000:ii;
      mean_time +=  (speed::diff_TSC-mean_time)/alpha;
      cout<<"SYMBOL       "<<C_Ofdm->symCounter<<endl;
      cout<<"CLOCK        "<<mean_time<<"("<<speed::diff_TSC<<")"<<endl;
      cout<<"Cyles/Symbol "<<mean_time*(ii+1)/C_Ofdm->symCounter<<endl;
    }
  while(1);

  delete C_Outer;
  delete C_Inner;
  delete C_InnerIntrlv;

  delete Msc_GenTs;
  delete M_Outer;
  delete M_Inner;
  delete M_InnerIntrlv;
  delete Dump2file;

  pApp.close();
  return -1;
}

int main(int argc, char **argv)
{
  // /* Input   : 4Mbits/s ==> 2660 mpeg8pcks/s           */
  // /* Resource: 1.6Ghz   ==> 600.000 Cycles/mpeg8pcks   */

  // QApplication a(argc, argv);
  // SdrGui *  pGui1 = new SdrGui();
  
  // SdrThread *plot1 = new SdrThread(pGui1,&main_dvbgen); 
  // plot1->SetRealTime(true);
  // plot1->start(); 

  // return a.exec();
  
  main_dvbgen(argc,argv);
  // sta::dumpMemberList();
  return -1;
}
