#include "sig_func.hh"
void sse_real_fc(Sigfl &output, const Sigcxs & input ){
  int input_sz   = input.size();
  int nbRemain16 = input_sz&15 ; int nb16 = input_sz   - nbRemain16 ;			
  int nbRemain4  = nbRemain16&3; int nb4  = nbRemain16 - nbRemain4  ;  

  int output_sz = output.size();
  
  if(output_sz==0){
    output.setsize(input_sz);
    output_sz = input_sz;
  }

#ifdef BOUNDSCHECKING
  int output_sz = output.size();			
  if(output_sz!=input_sz)				
    {								
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	
      myabort();						
    };								
#endif

  float  *adrI = (float*)input.getptr();
  float  *adrO = output.getptr();

  for(int ii=0;ii<nb16;ii+=8){
    *((__m128*)(adrO  ))  = _mm_shuffle_ps(*((__m128*)(adrI)),
					   *((__m128*)(adrI+4)),
					   _MM_SHUFFLE(2,0,2,0));
    *((__m128*)(adrO+4))  = _mm_shuffle_ps(*((__m128*)(adrI+8)),
					   *((__m128*)(adrI+12)),
					   _MM_SHUFFLE(2,0,2,0));
    adrI += 16;
    adrO += 8; 
  }
  for(int jj=0;jj<nb4;jj+=4){
    *((__m128*)(adrO  )) = _mm_shuffle_ps(*((__m128*)(adrI)),
					  *((__m128*)(adrI+4)),
					  _MM_SHUFFLE(2,0,2,0));
    adrI += 8;
    adrO += 4; 
  }
  for(int kk=0;kk<nbRemain4;kk++){
    *(adrO) = *(adrI);
    adrI+=2;   
    adrO+=1; 
  }
};

OPConv<Sigcxs,Sigfl> real(const Sigcxs & input ){
  OPConv<Sigcxs,Sigfl> op (&sse_real_fc,input);
  return op;	
};

/*******************************************************************************/
void sse_imag_fc(Sigfl &output, const Sigcxs & input ){
  int input_sz   = input.size();
  int nbRemain16 = input_sz&15 ; int nb16 = input_sz   - nbRemain16 ;			
  int nbRemain4  = nbRemain16&3; int nb4  = nbRemain16 - nbRemain4  ;  

  int output_sz = output.size();
  
  if(output_sz==0){
    output.setsize(input_sz);
    output_sz = input_sz;
  }

#ifdef BOUNDSCHECKING
  int output_sz = output.size();			
  if(output_sz!=input_sz)				
    {								
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	
      myabort();						
    };								
#endif

  float  *adrI = (float*)input.getptr();
  float  *adrO = output.getptr();

  for(int ii=0;ii<nb16;ii+=8){
    *((__m128*)(adrO  )) = _mm_shuffle_ps(*((__m128*)(adrI)),
					  *((__m128*)(adrI+4)),
					  _MM_SHUFFLE(3,1,3,1));
    *((__m128*)(adrO+4)) = _mm_shuffle_ps(*((__m128*)(adrI+8)),
					  *((__m128*)(adrI+12)),
					  _MM_SHUFFLE(3,1,3,1));
    adrI += 16;
    adrO += 8; 
  }
  for(int jj=0;jj<nb4;jj+=4){
    *((__m128*)(adrO  )) = _mm_shuffle_ps(*((__m128*)(adrI)),
					  *((__m128*)(adrI+4)),
					  _MM_SHUFFLE(3,1,3,1));
    adrI += 8;
    adrO += 4; 
  }
  for(int kk=0;kk<nbRemain4;kk++){
    *(adrO) = *(adrI+1);
    adrI+=2;   
    adrO+=1; 
  }
};

OPConv<Sigcxs,Sigfl> imag(const Sigcxs & input ){
  OPConv<Sigcxs,Sigfl> op (&sse_imag_fc,input);
  return op;	
};
