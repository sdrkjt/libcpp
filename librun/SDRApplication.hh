#ifndef SDRApplication_hh
#define SDRApplication_hh

#include "SDRModule.hh"
#include "libsig.hh"

class SDRApplication {
public:
  vector<SDRModule*> list_module;
  vector<SDRBuffer*> list_buffer;

  void add_module(SDRModule* psdrmod){
    list_module.push_back(psdrmod);
  };

  void connect(int nbarg,...){
    va_list ap;
    va_start(ap, nbarg);
    vector<SDRModule*> list_arg;

    for(int ii = 0 ; ii < nbarg ; ii++){
      SDRModule* pMod        = va_arg(ap, SDRModule*);
      list_arg.push_back(pMod);
    }

    for(int ii = 0 ; ii < nbarg-1 ; ii++){
      int output_sz = list_arg[ii]->get_o_sz();
      if(ii>0){
	int last_buf_sz = (list_buffer.back())->size();
	//if(last_buf_sz>output_sz) output_sz = last_buf_sz;
	output_sz = (last_buf_sz/list_arg[ii]->get_i_sz()) * output_sz;
      }
      int input_sz  = list_arg[ii+1]->get_i_sz();
      int buf_sz    = (output_sz==input_sz) ? (output_sz) : (output_sz+input_sz-1);
 
      SDRBuffer* pbuf = new SDRBuffer(buf_sz);
      list_arg[ii  ]->set_o_buf(pbuf);
      list_arg[ii+1]->set_i_buf(pbuf);
      //DUMP(output_sz); DUMP(input_sz);
      //DUMP(buf_sz);mygetchar()
      list_buffer.push_back(pbuf);
    }
  }

  void run(){
    int nb_mod = (int)list_module.size();
    for(int ii = 0 ; ii < nb_mod  ; ii++){
      list_module[ii]->start();
    }
  }

  void close(){
    for(int ibuf=0; ibuf< (int)list_buffer.size();ibuf++){
      delete list_buffer[ibuf];
    };
  }
};



#endif
