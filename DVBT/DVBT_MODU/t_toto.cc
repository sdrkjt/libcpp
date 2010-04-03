#include "dvbgen_prbs.hh"
#include "dvbgen_rs.hh"
#include "dvbgen_byteintrlv.hh"
#include "dvbgen_inner_coder.hh"

void get_mpeg_8pck(Sigui8 &O_mpeg8pck){
  CHECK_INIT_SIG(O_mpeg8pck,8*188);

  /*--DATA Bytes--*/
  O_mpeg8pck.random(8*188);

  /*--SYNC Bytes--*/
  for(int isync = 0;isync<1503;isync+=188)
    O_mpeg8pck[isync] = 0x47;
}
//mpeg pck 188
//rs   pck 204
        
int main_dvbgen(int argc, char **argv)
{
  enum {QPSK=2, QAM16=4, QAM64=6};

  int P_crate  = DvbParam::CR23;
 
  int sz_8pck_ts    = 1504; //size in byte
  int sz_8pck_outer = 1632; 
  int sz_8pck_inner = sz_8pck_outer * (P_crate+1)/P_crate;
  int sz_inner_input= 1260; //bytes 
  int Nmax = 1512 ;//2K; 6048 8K //nb subcarriers
  int nqam = QAM64;//4bits, 64


  int  nbit_info_hp = (int) ((float)(Nmax * nqam * P_crate) / ( (float) (P_crate+1))) ;
 

  /* CONVENTION:                                             */
  /* array index <--> time                                   */
  /* X[0]->X[1]->....->X[n], X[0] commes first then X[1] ... */

  //memset(sta::MEM,0,MAXSIZE);

  /*test alloca*/
  //sta::MEM = (char*)alloca(MAXSIZE);

  /****INIT PARAM*****/
  Sigui8 O_mpeg8pck        ;O_mpeg8pck     .SetSize(1504); /* TS output */
  Sigui8 O_rs8pck          ;O_rs8pck       .SetSize(1632); /* RS_INTRLV output */
  Sigui8 tmp1pck           ;tmp1pck        .SetSize(204 ); /* used for RS_INTRLV encoding */
  
  //[TS]->buf0->[Scrambler]->buf1->[RS+ByteInterleaver]->buf2->[Inner Coder]

  /*THREAD
    Module: read_request(); write_request();
    Buffer: read_respond(); write_respond();
    Module: m_run();// check inputs, if ok then throw compute() and write_request(), if not, do nothing

    m_run_thread()
    {
    DO{
    read_request();    //demand to read from buffer    
    wait_read();       //wait until buffer responds ok then copy datas from buffer to input memory
    compute();         //process datas, results are stored in output memory
    write_request();   //demand to write to buffer
    wait_write();      //wait until buffer responds ok then copy output memory to buffer
    }WHILE(1);
    }
  */

  /*NO THREAD
    Also need extra memory to store input and output 
    Module: CBuffer * I_buffer; CBuffer * O_buffer; 

    SDRApplication newApp;
    newApp.connect(modu1,modu2,modu3);

    newApp.run(){
    DO
    {
    modu1->m_run_nothread();
    modu2->m_run_nothread();
    modu3->m_run_nothread();
    }WHILE(1);
    }

    m_run_nothread()
    {
    WHILE (read_ok(Ls)) DO {

    SigT I_sig1;  I_buffer->data(ird,ird+Ls-1);
    compute(I_sig1);
    write_request();
    }
  */






  /*----Scrambler Init--------------*/  
  Sigui8 O_Scrmb;O_Scrmb.SetSize(1504);
  TIME();
  dvb_gen_scrmb_1504(O_Scrmb);
  TIME("dvb_gen_scrmb_1503");
  DUMP(O_Scrmb);  figure(1);plot1d(O_Scrmb,"O_Scrmb");

  /*----RS+ByteInterleaver Init-----*/
  RS256 M_rsencoder;

  /*----Byte Interleaver Init-------*/
  ByteIntrlv M_byte_intrlv;

  /*----Inner Coder      Init-------*/
  InnerCoder M_inner_coder(P_crate);

  /****END INIT*******/

  LOOP(ii,10)
    {
      /****Gen mpeg 8pcks********************/
      get_mpeg_8pck(O_mpeg8pck);
      DUMP(O_mpeg8pck);  figure(2);plot1d(O_mpeg8pck, "O_mpeg8pck");

      /****Scrambler*************************/
      //-bit-wise inverted the first SYNC in mpeg8pck -//
      O_mpeg8pck(0)= ~0x47;
      TIME();
      O_mpeg8pck = O_mpeg8pck ^ O_Scrmb;
      TIME("Scrambler 8pcks");                                /* 2.700Cycles/mpeg8pck */
      DUMP(O_mpeg8pck); figure(3);plot1d(O_mpeg8pck, "O_mpeg8pck scramblé");

      /****RS Encoding + Byte Interleaving***/
      TIME();
      LOOP(ipck,8){
	int rs_ptr = ipck*204; int mpeg_ptr = ipck*188;
	tmp1pck(0,187) =  O_mpeg8pck(mpeg_ptr,mpeg_ptr+187);

	M_rsencoder  .encode1pck(tmp1pck);
	M_byte_intrlv.do_intrlv_1pck(O_rs8pck(rs_ptr,rs_ptr+203),
				     tmp1pck                     );
      }
      TIME("(RSencoder + Byte Interleaving) 8pcks");         /* 51.000(45.000+15.000)cycles/mpeg8pck */
                                                             /* 45.000 without getchar               */

      DUMP(O_rs8pck);  figure(10); plot1d(O_rs8pck,"O_rs8pck");

      /****Convolutional Encoder*************/
      if (P_crate==DvbParam::CR12)
      {
	//uint8 O_X,O_Y; 
	uint16 O_XY;
	TIME();
	for(int ibyte=0;ibyte<1632;ibyte++)
	  {
	    //M_inner_coder.encode1byte(O_X,O_Y,O_rs8pck(ibyte));
	    M_inner_coder.encode_punture_C12(O_XY,O_rs8pck(ibyte));
	  }
	TIME("Convolutional Encoder 8pcks");                   /* 29.000 cycles/8pcks    */               
	                                                       /* 12.000 array, 18.000 pointer, without getchar */ 
	//DUMP((int)O_X);
	//DUMP((int)O_Y);
	//DUMP((int)O_XY);
	DUMPBIT(int,O_XY);
      }
      else if (P_crate==DvbParam::CR23)
      {
	uint24 O_XY;
	TIME();
	for(int ibyte=0;ibyte<1632/2;ibyte++)
	  {
	    M_inner_coder.encode_punture_C23(O_XY,O_rs8pck(ibyte<<1),O_rs8pck((ibyte<<1)+1));
	  }
	TIME("Convolutional Encoder 8pcks");                  
	DUMPBIT(int,O_XY); 
  
	Sigui8 O_vecbyte; O_vecbyte.SetSize(1890);
	Sigui8 I_vecbyte; I_vecbyte = O_rs8pck(0,1259);//DUMP(I_vecbyte);
	TIME();
	M_inner_coder.encode_punture_1260_C23(O_vecbyte, I_vecbyte);
	TIME("Convolutional Encoder 1260Bytes"); 
	DUMP(O_vecbyte);
      }

    }


  //check memalloc
  if(0){
    sta::dumpMemberList();
    mygetchar();
  }

  return -1;
}

int main(int argc, char **argv)
{
  /* Input   : 4Mbits/s ==> 2660 mpeg8pcks/s           */
  /* Resource: 1.6Ghz   ==> 600.000 Cycles/mpeg8pcks   */

  // QApplication a(argc, argv);
  // SdrGui *  pGui1 = new SdrGui();
 
  // SdrThread *plot1 = new SdrThread(pGui1,&main_dvbgen);
  // plot1->start();

  // return a.exec();

  main_dvbgen(argc,argv);
  return -1;
}
