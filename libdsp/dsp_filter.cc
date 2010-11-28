#include "dsp_filter.hh"

dsp_filter::dsp_filter(const Sigfl &coef, int vnbIput){
  nbCoef  = coef.size();
  nbIput  = vnbIput;

  coefIQ.setsize(nbCoef);
  for(int ii=0;ii<nbCoef;ii++){
    coefIQ[ii] = Cxs(coef[ii],0);
  }
 
  int nbCoef_align = ((nbCoef+3+4)>>2)<<2;
  int start;

  Sigfl coefVect;  coefVect.zeros(4*nbCoef_align);
  for(int kk=0;kk<4;kk++){
    start = nbCoef_align*kk;
    coefVect(start+kk,start+kk+nbCoef-1) = coef;
  }

  int size = coefVect.size()/4;
  for(int ii=0;ii<4;ii++)coefMat[ii] = coefVect(ii*size,ii*size+size-1);

  /*init buffer data*/
  int datbufsz = (nbCoef-1)+nbIput+(nbCoef_align-nbCoef);
  dataBuffer.zeros(datbufsz);
  Sigcxs coef_zero; coef_zero.zeros(datbufsz);
  coef_zero[0] = Cxs(coef[0],0);
  for(int ii=0;ii<nbCoef-1;ii++)coef_zero[datbufsz-nbCoef+ii+1] = Cxs(coef[nbCoef-ii-1],0);

  coefBuffer_fft = dft(coef_zero);
};

#define  motif_sdot(ii,tmp,accu) tmp =   _mm_mul_ps( *((__m128*)(adrx+ii)), *((__m128*)(adry+ii)) ) ; \
    accu =  _mm_add_ps(tmp, accu);					\

void dsp_filter::sse_compute(Sigcxs &O_dat,const Sigcxs &I_dat){
  int szOput  = O_dat.size();						
  if(szOput == 0){									    
    O_dat.setsize(nbIput);						
    szOput = nbIput;
  }

#ifdef BOUNDSCHECKING
  int szIput  = I_dat.size();
  if(szIput!=nbIput || szOput!=nbIput){
    MSG_ERROR("Size mismatch");
    myabort();
  }
#endif
  /*copy data to the data buffer*/
  dataBuffer(nbCoef-1,nbCoef+nbIput-2) = I_dat;

  int nbCoef_align = ((nbCoef+3+4)>>2)<<2;
  O_dat = dataBuffer(0,nbIput-1);
  int ird = 0;  int iwr = 0;
  int ifir,idat;

  for(int ii=0;ii<nbIput;ii++){
    ifir = (ii&3);  idat = ii-ifir;
    O_dat[ii] = sse_dotproduct_ccf(dataBuffer(idat,idat+nbCoef_align-1),coefMat[ifir]);
  }

  /*prepare data buffer for the next compute*/
  dataBuffer(0,nbCoef-2) = dataBuffer(nbIput,nbCoef+nbIput-2);
}

void dsp_filter::fft_compute(Sigcxs &O_dat,const Sigcxs &I_dat){
  int szOput  = O_dat.size();						
  if(szOput == 0){									    
    O_dat.setsize(nbIput);						
    szOput = nbIput;
  }

#ifdef BOUNDSCHECKING
  int szIput  = I_dat.size();
  if(szIput!=nbIput || szOput!=nbIput){
    MSG_ERROR("Size mismatch");
    myabort();
  }
#endif
  /*copy data to the data buffer*/
  dataBuffer(nbCoef-1,nbCoef+nbIput-2) = I_dat;

  Sigcxs tmp1,tmp2,tmp3,tmp4;
  tmp1 = dft(dataBuffer);  
  tmp3 = tmp1*coefBuffer_fft;
  tmp4 = idft(tmp3);

  O_dat = tmp4(0,szOput-1);

  /*prepare data buffer for the next compute*/
  dataBuffer(0,nbCoef-2) = dataBuffer(nbIput,nbCoef+nbIput-2);
}

void dsp_filter::cpp_compute(Sigcxs &O_dat,const Sigcxs &I_dat){
  int szOput  = O_dat.size();						
  if(szOput == 0){									    
    O_dat.setsize(nbIput);						
    szOput = nbIput;
  }

#ifdef BOUNDSCHECKING
  int szIput  = I_dat.size();
  if(szIput!=nbIput || szOput!=nbIput){
    MSG_ERROR("Size mismatch");
    myabort();
  }
#endif
  /*copy data to the data buffer*/
  dataBuffer(nbCoef-1,nbCoef+nbIput-2) = I_dat;

  O_dat.zeros(szOput);

  for(int ii=0;ii<szOput;ii++){
    for(int jj=0;jj<nbCoef;jj++){
      O_dat[ii] = O_dat[ii]+dataBuffer[ii+jj] * coefIQ[jj];
    }
  }

  /*prepare data buffer for the next compute*/
  dataBuffer(0,nbCoef-2) = dataBuffer(nbIput,nbCoef+nbIput-2);
}
