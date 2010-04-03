/******************************************
 *    
 *    
 ******************************************/
ofstream ofile;
int main (Int argc , char ** argv) 
{
  if (argc < 2) 
    {
      cout << "Usage :        ~/$ pattern_gen filename [OPTIONS]\n";
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
    }


  /********** Simu Parameters *********************/

  //--- Parameters ---
  Int  nfft           = 0;
  Int  guard          = 0;
  Int  nqam           = 2;
  Int  native_intlv   = 1;
  Int  hrch           = 0;
  Int  select_hp      = 1;
  Int  alpha          = 1;
  Int  code_rate_hp   = 2;
  Int  code_rate_lp   = 2;  
  Int seed = time(NULL);
  char * filename = argv[1];
 for (Int i=2 ; i<argc ; i++) {
   if (strcmp(argv[i],"-nfft"         )==0)  nfft           = atoi(argv[i+1]); 
   if (strcmp(argv[i],"-guard"        )==0)  guard          = atoi(argv[i+1]); 
   if (strcmp(argv[i],"-nqam"         )==0)  nqam           = atoi(argv[i+1]); 
   if (strcmp(argv[i],"-intlv"        )==0)  native_intlv   = atoi(argv[i+1]);
   if (strcmp(argv[i],"-hrch"         )==0)  {hrch          = (atoi(argv[i+1]) != 0);  select_hp      = (atoi(argv[i+1]) != 2); }
   if (strcmp(argv[i],"-alpha"        )==0)  alpha          = atoi(argv[i+1]);
   if (strcmp(argv[i],"-code_rate_hp" )==0)  code_rate_hp   = atoi(argv[i+1]);
   if (strcmp(argv[i],"-code_rate_lp" )==0)  code_rate_lp   = atoi(argv[i+1]);
   if (strcmp(argv[i],"-seed"         )==0)  seed = atoi(argv[i+1]);
 }

  cerr << "\nPattern generation using:\n";
  cerr << "   filename       = " << filename  << "\n";
  cerr << "   nfft           = " << ((nfft==0)?"2k":((nfft==2)?"4k":"8k")) << "\n";
  cerr << "   guard          = 1/" << 32/(1<<guard) << "\n";
  cerr << "   nqam           = " << ((nqam==0)?"QPSK":((nqam==1)?"16QAM":"64QAM")) << "\n";
  cerr << "   interleaver    = " << ((native_intlv==0)?"extended":"native")  << "\n";
  cerr << "   hrch           = " << ((hrch==1) ? "hierarchical":"non-hierarchical") << "\n";
  cerr << "   alpha          = " << alpha << "\n";
  cerr << "   code_rate_hp   = " << code_rate_hp << "/"<< code_rate_hp+1 << "\n";
  cerr << "   code_rate_lp   = " << code_rate_lp << "/"<< code_rate_lp+1 << "\n";
  cerr << "   Random Seed    = " << seed << "\n";
 srandom(seed);


 ofile = new ofstream (filename, ios::out);ofile->setf(ios::left);


  //------------- COFDM signal computation --------------
 Int Lu = ((nfft==0)?2048:((nfft==2)?4096:8192));
 Int Ls = Lu + Lu * (1<<guard) / 32;
 Int cellid        = 1;
 Int cellid_val    = 0;
 Int isym          = 0;
 Int intlv_sym_max = 1;
 Int L             = 4*68*Ls;
 Real mean = 0.0, max = 0.0;
 Complex *  mem_vectLs   = new Complex [L];
 dvb_gensym_ofdm * u_gensym_ofdm = new dvb_gensym_ofdm("u_gensym_ofdm"); 
 if(native_intlv==0) {
   if(nfft == 0) intlv_sym_max = 4;
   else if(nfft == 2) intlv_sym_max = 2;
   else intlv_sym_max = 1;
 }


  fprintf(stderr,"\nCOFDM data generation \n");
  for(isym=0;isym<4*68;isym+=intlv_sym_max)
  u_gensym_ofdm->vectproc (***);


  //------------- output filtering --------------
  fprintf(stderr,"\nOutput filtering \n");
  mean = 0.0;  for(int k=0;k<L;k++) mean += norm(mem_vectLs[k]);
  mean /= L;mean =sqrt(mean);  for(int k=0;k<L;k++) mem_vectLs[k] /= mean;// 0 dB

  Int fc = L/2*8/8192*896;
  Int radix;
  if(guard==0)  	radix =33;
  else if(guard==1) radix =17;
  else if(guard==2) radix =9;
  else            	radix =5;

  L/=128;
 Complex *  mem   = new Complex [L];
 Complex *  mem2   = new Complex [L];
 memcpy(mem,mem_vectLs,L*sizeof(Complex));
  fft ( mem_vectLs , L , -1 , radix);
 //fft ( mem_vectLs , L ,  1 , radix);
  mean = 0.0;  for(int k=0;k<L;k++) mean += norm(mem_vectLs[k]);
  mean /= L;mean =sqrt(mean);  for(int k=0;k<L;k++) mem_vectLs[k] /= mean;// 0 dB
  for(int k=0;k<L;k++) for(int n=0;n<L;n++)
		mem2[k] += mem[n]*exp(Complex(0.0,-2*PI*n*k/L));
  mean = 0.0;  for(int k=0;k<L;k++) mean += norm(mem2[k]);
  mean /= L;mean =sqrt(mean);  for(int k=0;k<L;k++) mem2[k] /= mean;// 0 dB
  fprintf(stderr,"\nWrite output file\n");
  for(int k=0;k<L;k++)  
	*ofile << 10*log10(norm(mem_vectLs[k]-mem[k])+1e-10)-10*log10(norm(mem[k])+1e-10) << " " << mem_vectLs[k] << " " << mem[k]  << "\n";
  ofile->close();
  exit(1);

  fprintf(stderr,"  Direct FFT \n");
  fft ( mem_vectLs , L , -1 , radix);

  fprintf(stderr,"  Windowing at %gMHz\n",((Real)fc)/L*8192/896);
  for(int k=fc;k<L-fc;k++) mem_vectLs[k]=0.0;

  mean = 0.0;  for(int k=0;k<L;k++) mean += norm(mem_vectLs[k]);
  mean /= L;mean =sqrt(mean);  for(int k=0;k<L;k++) mem_vectLs[k] /= mean;// 0 dB

 Complex *  mem_vect   = new Complex [L];
 memcpy(mem_vect,mem_vectLs,L*sizeof(Complex));

  fprintf(stderr,"  Inverse FFT \n");
  fft ( mem_vectLs , L , 1 , radix);

  mean = 0.0;  for(int k=0;k<L;k++) mean += norm(mem_vectLs[k]);
  mean /= L;mean =sqrt(mean);  for(int k=0;k<L;k++) mem_vectLs[k] /= mean;// 0 dB

  //---------------- normalisation ----------------
 Complex *  mem_vect2   = new Complex [L];
 memcpy(mem_vect2,mem_vectLs,L*sizeof(Complex));
  fprintf(stderr,"\nOutput normalization\n");
  for(int k=0;k<L;k++)  {
	if(max < fabs(real(mem_vectLs[k]))) max = fabs(real(mem_vectLs[k]));
	if(max < fabs(imag(mem_vectLs[k]))) max = fabs(imag(mem_vectLs[k]));
  }
  fprintf(stderr,"  Peak to average ratio: %gdB\n",20*log10(max));
  for(int k=0;k<L;k++) {
	mem_vectLs[k] /= max;
	Real dr = rint(32768*real(mem_vectLs[k]));
	Real di = rint(32768*imag(mem_vectLs[k]));
	if(dr>=32768) dr = 32767;
	if(dr<-32768) dr = -32768;
	if(di>=32768) di = 32767;
	if(di<-32768) di = -32768;
	mem_vectLs[k] = Complex(dr,di);
  }


  fprintf(stderr,"  Direct FFT \n");
  fft ( mem_vectLs , L , -1 , radix);
  //fft ( mem_vect , L , -1 , radix);
  fprintf(stderr,"\nWrite output file\n");
  for(int k=0;k<L;k++)  
	*ofile <<  10*log10(norm(mem_vectLs[k]/32768-mem_vect[k]/max)) << " " << 10*log10(norm(mem_vect[k]/max)) << " " << 10*log10(norm(mem_vectLs[k]/32768)) << "\n";
  //*ofile <<  10*log10(norm(mem_vectLs[k]/32768)) <<  "\n";
  ofile->close();
  exit(1);
	


  //------------- Write output file --------------
  fprintf(stderr,"\nWrite output file\n");
  for(int k=0;k<L;k++)  {
	Int dr = (Int)rint(real(mem_vectLs[k]));
	Int di = (Int)rint(imag(mem_vectLs[k]));
	*ofile << (char) ((dr>>8)&0xff) ;
	*ofile << (char) (dr&0xff) ;
	*ofile << (char) ((di>>8)&0xff) ;
	*ofile << (char) (di&0xff) ;
  }
  ofile->close();
}







