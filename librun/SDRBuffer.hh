#ifndef SDRBuffer_hh
#define SDRBuffer_hh

#include "cpp_monit.hh"

#define BUFFER_NAME(x) { x.name = #x; }

class SDRBuffer {
public:
  string name;

  SDRBuffer(const int &sz){
    buf_sz = sz;
    cur_sz = 0;  //current size
    wr_idx = 0;  //write index
    rd_idx = 0;  //read index

    data.SetSize(sz);
  }

  bool get_sample(Sigui8 & I_data){
    int get_sz =  I_data.size();
    bool is_available = (cur_sz >= get_sz);
    if(is_available){  
      int sz1 = buf_sz-rd_idx;
      int sz2 = get_sz-sz1;
      if( sz2>0 ){
	/*copy data from buffer*/
	I_data(0,sz1-1) = data(rd_idx,buf_sz-1);
	I_data(sz1, get_sz-1) = data(0,sz2-1);
	/*change read index*/
	rd_idx  = sz2; //rd_idx += get_sz - buf_sz
      }
      else{
	/*copy data from buffer*/
	I_data = data(rd_idx,rd_idx+get_sz-1);
	/*change read index*/
	rd_idx += get_sz;
      }
      /*shrink current size*/
      cur_sz -= get_sz; 
    }
    return is_available;
  };

  void put_sample(const Sigui8 & I_data){
    int  put_sz = I_data.size();
    bool is_overflow = (put_sz>buf_sz-cur_sz);
    if(!is_overflow){
      int sz1 = buf_sz-wr_idx;
      int sz2 = put_sz-sz1;
      if( sz2>0 ){
	/*write data to buffer*/
	data(wr_idx,buf_sz-1) = I_data(0,sz1-1);
	data(0,sz2-1)         = I_data(sz1, put_sz-1);
	/*change write index*/
	wr_idx  = sz2; //wr_idx += put_sz - buf_sz
      }
      else{
	/*write data to buffer*/
	data(wr_idx,wr_idx+put_sz-1) = I_data;
	/*change write index*/
	wr_idx += put_sz;
      }
      /*expand current size*/
      cur_sz += put_sz; 
    }
    else{
      DUMP(put_sz);DUMP(buf_sz);DUMP(cur_sz);
      MSG_ERROR("Buffer Overflow!! "<<name);
    }
  };
  int size(){
    return buf_sz;
  };
  void set_name(string sname){
    name = sname;
  }

private:
  Sigui8 data;
  int buf_sz; //buffer size
  int cur_sz; //current size
  int wr_idx; //write index
  int rd_idx; //read index
};

#endif
