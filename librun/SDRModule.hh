#ifndef SDRModule_hh
#define SDRModule_hh

#include "SDRBuffer.hh"

#define MODULE_NAME(x){(x).name = #x;}

#ifdef TIME_CORE
#define START_TIMER_CORE()  TIME();
#else 
#define START_TIMER_CORE()  
#endif		

#ifdef TIME_CORE		
#define STOP_TIMER_CORE(name)  TIME("[CORE] "+name);		
#else
#define STOP_TIMER_CORE(name) 
#endif


class SDRModule{
public:
  string name;

  void set_i_buf(SDRBuffer *vI_buffer){
    I_buffer = vI_buffer;
  };
  void set_o_buf(SDRBuffer *vO_buffer){
    O_buffer = vO_buffer;
  };

  int get_i_sz(){
    return nbInputItems;
  };
  int get_o_sz(){
    return nbOutputItems;
  };

  virtual void m_run(){
    MSG_ERROR("m_run() not defined"); 
  };

  //function for thread
  void start(){
    //do{
#ifdef TIME_MRUN
    TIME();
#endif

    m_run();

#ifdef TIME_MRUN
    TIME("[MRUN] "+name);
#endif
    //}while(1);
  }
  virtual ~SDRModule(){};

protected:
  SDRBuffer* I_buffer; 
  SDRBuffer* O_buffer;

  int    nbInputItems;
  int   nbOutputItems;

};

template<typename ModType>
class SDRMethCore: public SDRModule{
public:
  SDRMethCore(ModType *v_pmod,void (ModType:: *v_pmeth)(Sigui8 &,  Sigui8&),const int &v_nbInputItems, const int &v_nbOutputItems){
    pmod = v_pmod; pmeth = v_pmeth;
    nbInputItems  = v_nbInputItems;
    nbOutputItems = v_nbOutputItems;

    I_data.SetSize(nbInputItems);
    O_data.SetSize(nbOutputItems);

    I_buffer = NULL; O_buffer = NULL;
  }

  void m_run(){
    while( I_buffer->get_sample(I_data) ){
      
      START_TIMER_CORE();

      (pmod->*pmeth)(O_data,I_data);

      STOP_TIMER_CORE(name);

      O_buffer->put_sample(O_data);	
    }
  };

private:
  ModType* pmod;
  void (ModType:: *pmeth) (Sigui8 &,  Sigui8&);

  Sigui8  I_data;
  Sigui8  O_data;

};

class SDRFuncCore: public SDRModule {
public:
  SDRFuncCore(void (*v_pfunc)(Sigui8 &,  Sigui8&),const int &v_nbInputItems, const int &v_nbOutputItems){
    pfunc = v_pfunc;
    nbInputItems  = v_nbInputItems;
    nbOutputItems = v_nbOutputItems;

    I_data.SetSize(nbInputItems);
    O_data.SetSize(nbOutputItems);

    I_buffer = NULL; O_buffer = NULL;
  }

  void m_run(){
    while( I_buffer->get_sample(I_data) ){

      START_TIMER_CORE();

      (*pfunc)(O_data,I_data);

      STOP_TIMER_CORE(name);

      O_buffer->put_sample(O_data);	
    }
  };

private:
  void (*pfunc) (Sigui8 &,  Sigui8&);

  Sigui8  I_data;
  Sigui8  O_data;

};

class SDRFuncSource: public SDRModule {
public:
  SDRFuncSource(void (*v_pfunc)(Sigui8  &), const int &v_nbOutputItems) {
    nbOutputItems = v_nbOutputItems; nbInputItems = 0;
    O_data.SetSize(nbOutputItems);
    O_buffer = NULL; pfunc = v_pfunc;
  }

  void m_run(){

    START_TIMER_CORE();

    (*pfunc)(O_data);
    
    STOP_TIMER_CORE(name);

    O_buffer->put_sample(O_data);
  };

private:
  void (*pfunc)(Sigui8  &);

  Sigui8  O_data;
};

template<class ModType>
class SDRMethSource: public SDRModule {
public:
  SDRMethSource(ModType* v_pmod, void (ModType::*v_pmeth)(Sigui8  &), const int &v_nbOutputItems){
    nbOutputItems = v_nbOutputItems; nbInputItems = 0;
    O_data.SetSize(nbOutputItems);
    O_buffer = NULL; 
    pmod = v_pmod; pmeth = v_pmeth;
  }

  void m_run(){
    START_TIMER_CORE();

    (pmod->*pmeth)(O_data);

    STOP_TIMER_CORE(name);

    O_buffer->put_sample(O_data);
  };

private:
  ModType            * pmod;
  void  (*pmeth)(Sigui8  &);

  Sigui8             O_data;

};

class SDRFifoSource: public SDRModule {
public:
  SDRFifoSource(const char* vfilename, const int &v_nbOutputItems) {
    nbOutputItems = v_nbOutputItems; nbInputItems = 0;
    O_data.SetSize(nbOutputItems);
    O_buffer = NULL;

    sprintf (filename, "%s", vfilename);
    pFile = fopen ( filename , "rb" );
    if (pFile==NULL) {fputs ("SDRFifoSource: can not open file\n",stderr); exit (1);}
  }

  ~SDRFifoSource(){
    fclose(pFile);
  }

  void m_run(){
    START_TIMER_CORE();
    fread (O_data.bufferA,1,nbOutputItems,pFile);
    STOP_TIMER_CORE(name);

    O_buffer->put_sample(O_data);
  };

private:
  char filename[100];
  FILE * pFile;

  Sigui8  O_data;

};

class SDRFileSource: public SDRModule {
public:
  SDRFileSource(const char* vfilename, const int &v_nbOutputItems,  string rtype_v="") {
    nbOutputItems = v_nbOutputItems; nbInputItems = 0;
    O_data.SetSize(nbOutputItems);
    O_buffer = NULL;

    sprintf (filename, "%s", vfilename);

    if(rtype_v=="")             rtype = DEFAULT;
    else if (rtype_v=="flush" ) rtype = FLUSH;
    else if (rtype_v=="circul") rtype = CIRCUL;
    else {
      MSG_ERROR("Reading Mode Undefined << "<<rtype_v);
    }

    FILE * pFile;
    pFile = fopen ( filename , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    fileSize = ftell (pFile);
    rewind (pFile);
    // allocate memory to contain the whole file:
    pfile = (uint8*) malloc (fileSize);
    // copy the file into the buffer:
    size_t result = fread (pfile,1,fileSize,pFile);
    if (result != fileSize) {fputs ("SDRFileSource: Reading error\n",stderr); exit (3);}
    fclose (pFile);

    iRead = 0;
  }

  ~SDRFileSource(){
    free(pfile);
  }

  void m_run(){
    if(iRead+nbOutputItems<fileSize){
      START_TIMER_CORE();
      
      memcpy(O_data.bufferA,pfile+iRead,nbOutputItems);
      iRead +=  nbOutputItems; 
      
      STOP_TIMER_CORE(name);

      O_buffer->put_sample(O_data);
    }
    else{
      int Navailable = fileSize-iRead;
      if(rtype==FLUSH)
	{
	  if(Navailable>0){
	    START_TIMER_CORE();

	    memcpy(O_data.bufferA,pfile+iRead,Navailable);
	    memset(O_data.bufferA+Navailable,0,nbOutputItems-Navailable);
	    iRead = fileSize;
	    
	    STOP_TIMER_CORE(name);

	    O_buffer->put_sample(O_data);
	  }
	  else { MSG_WARNING("End Of File: "<<filename); mygetchar(); }
	}
      else if (rtype==CIRCUL)
	{
	  START_TIMER_CORE();

	  memcpy(O_data.bufferA,pfile+iRead,Navailable);
	  memcpy(O_data.bufferA+Navailable,pfile,nbOutputItems-Navailable);
	  iRead = nbOutputItems-Navailable;
	  
	  STOP_TIMER_CORE(name);

	  O_buffer->put_sample(O_data);
	}
      else{
      	MSG_WARNING("End Of File: "<<filename);  mygetchar(); 
      }

    }
  };

private:
  char filename[100];
  uint64 fileSize;
  uint64 iRead;
  uint8* pfile;
  int rtype;
  enum { DEFAULT, FLUSH, CIRCUL, FIFO };
  Sigui8  O_data;

};


class SDRFuncSink: public SDRModule {
public:
  SDRFuncSink(void (*v_pfunc)(Sigui8  &), const int &v_nbInputItems){
    nbInputItems  = v_nbInputItems; nbOutputItems = 0;
    I_data.SetSize(nbInputItems);
    I_buffer = NULL; pfunc = v_pfunc;
  }

  //virtual void m_run(){ MSG_WARNING("m_run undefined"); };
  void m_run(){
    while( I_buffer->get_sample(I_data) ){
      START_TIMER_CORE();

      (*pfunc)(I_data);

      STOP_TIMER_CORE(name);
    }
  };

private:
  void (*pfunc)(Sigui8  &);

  Sigui8       I_data;

};

template<class ModType>
class SDRMethSink: public SDRModule {
public:
  SDRMethSink(ModType* v_pmod, void (ModType::*v_pmeth)(Sigui8  &), const int &v_nbInputItems){
    nbInputItems  = v_nbInputItems; nbOutputItems = 0;
    I_data.SetSize(nbInputItems);
    I_buffer = NULL; 
    pmod = v_pmod; pmeth = v_pmeth;
  }

  void m_run(){
    while( I_buffer->get_sample(I_data) ){
      START_TIMER_CORE();

      (pmod->*pmeth)(I_data);

      STOP_TIMER_CORE(name);
    }
  };

private:
  ModType       *pmod;
  void (ModType::*pmeth)(Sigui8  &);

  Sigui8       I_data;

};

class SDRNullSink: public SDRModule {
public:
  SDRNullSink(const int &v_nbInputItems){
    nbInputItems  = v_nbInputItems; nbOutputItems = 0;
    I_data.SetSize(nbInputItems);
    I_buffer = NULL;
  }

  void m_run(){
    while( I_buffer->get_sample(I_data) ){
      //do nothing
    }
  };

private:
  Sigui8       I_data;

};

class SDRFileSink: public SDRModule {
public:
  SDRFileSink(const char* vfilename,const int &v_nbInputItems){
    nbInputItems  = v_nbInputItems; nbOutputItems = 0;
    I_data.SetSize(nbInputItems);
    I_buffer = NULL;

    sprintf(filename, "%s", vfilename);
    pFile = fopen (filename,"wb");
  }
  ~SDRFileSink(){
    fclose (pFile);
  }
  void m_run(){
    while( I_buffer->get_sample(I_data) ){
      fwrite(I_data.bufferA , 1 ,  nbInputItems, pFile );
    }
  };

private:
  char filename[100];
  FILE * pFile;

  Sigui8       I_data;

};
#endif
