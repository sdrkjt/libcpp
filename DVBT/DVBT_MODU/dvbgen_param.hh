#ifndef dvbgen_param_hh
#define dvbgen_param_hh

class DvbParam {
public:
  enum Mode {Mode2K=2048,Mode4K=4096,Mode8K=8192,ModeNull=0};
  enum Constel {QPSK=2,QAM16=4,QAM64=6};
  enum CodeRate {CR12=1,CR23=2,CR34=3,CR56=5,CR78=7,CRNull=0};

  DvbParam(){
    cellid       = false;
    cellidValue  = 0;
    constel      = QAM16;//0-1-2-3:QPSK, 16-QAM, 64-QAM, Reserved
    native_intlv = 1;//native vs in-depth symbol intrlv
    hierarch     = 0;//hierarchical vs non hierarchical
    alpha        = 1;//1 ,2 ,4
    crateH       = CR23; //1-2-3-5-7: 1/2;2/3;3/4;5/6;7/8
    crateL       = CR23;//CRNull;
    guard        = 4;//32,16,8,4
    Lu           = Mode2K;;//Mode8K;//Mode2K; //0-1-2-3 : 2k;8k;4k;Reserved
    Ku           = 1705;//1705(2k);6817(8k)
    Nmax         = 1512;//1512(2k);6048(8k)
    NbTps        = 17  ;//17  (2k);68  (8k)
  }

  bool        cellid       ;
  int         cellidValue  ;
  Constel     constel      ;//0-1-2-3:QPSK, 16-QAM, 64-QAM, Reserved
  int         native_intlv ;//native vs in-depth symbol intrlv
  int         hierarch     ;//hierarchical vs non hierarchical
  int         alpha        ;
  CodeRate    crateH       ;//1-2-3-5-7: 1/2;2/3;3/4;5/6;7/8
  CodeRate    crateL       ;
  int         guard        ;//32,16,8,4
  Mode        Lu           ;//2k;8k;4k;Reserved
  int         Ku           ;
  int         Nmax         ;
  int         NbTps        ;
};

#endif
