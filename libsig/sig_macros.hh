#ifndef sig_macros_hh
#define sig_macros_hh

#define CHECK_INIT_SIG(x,size)					\
  int initsize = x.GetSize();					\
  if(initsize==0)x.SetSize(size);				\
  else if(initsize!=size){					\
    cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	\
    myabort();							\
  }								\

#define LOOP(ii,nloop) for(int ii=0;ii<nloop;ii++)


#define EXTRACTBIT(input,n) ((input>>(n-1))&0x1) 
#define GETBIT(input,n) ((input>>(n-1))&0x1)
#define SETBIT(input,n,val) input = input + ( (val - ((input>>(n-1))&0x1))<<(n-1) )

#define FORLIST(list,ii) for(int ii=0;ii<(int)list.size();ii++)

#ifdef SigName
#define SIGNAME(S) S.name = "("+string(#S)+")"+S.name;
#else 
#define SIGNAME(S) 
#endif






inline string num2str(const int& num){
  ostringstream os;
  os<<num;
  string res; ;
  res = "" + os.str();
  return res;
}

#endif
