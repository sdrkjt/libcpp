#ifndef sig_eltwise_hh
#define sig_eltwise_hh

#include "AlignedArray.hh"
#include "sig_scalar.hh"
#include "sig_types.hh"

#ifdef BOUNDSCHECKING
#define ELTWISE_BOUNDSCHECKING()				\
  if(size_a!=size_b||size_a!=size_c)				\
    {								\
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	\
      myabort();						\
    };								\

#define ELTWISE_1_BOUNDSCHECKING()				\
  if(size_a!=size_c)						\
    {								\
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	\
      myabort();						\
    };								\

#else 
#define ELTWISE_BOUNDSCHECKING() 
#define ELTWISE_1_BOUNDSCHECKING() 
#endif

#ifdef STDCpp
#define use_stdc true
#else 
#define use_stdc false
#endif

#define DefBaseOp2(funId,opId)						\
  template <typename TX,typename VY>					\
  void funId (AATV& Sc, const AATV& Sa,  const AATV& Sb)		\
  {									\
    int size_a = Sa.GetSize();						\
    int size_b = Sb.GetSize();						\
    int size_c = Sc.GetSize();						\
    if(size_c == 0)							\
      {									\
	Sc.SetSize(size_a);						\
	size_c = size_a;						\
      }									\
    ELTWISE_BOUNDSCHECKING();						\
    TX* adrSa = Sa.bufferA;						\
    TX* adrSb = Sb.bufferA;						\
    TX* adrSc = Sc.bufferA;						\
    bool unaligned = (  (((long int)adrSa)&15)				\
			||(((long int)adrSb)&15)			\
			||(((long int)adrSc)&15) );			\
    WarningUnaligned(unaligned);					\
    const int ElementsPerVector = sizeof(VY) / sizeof(TX);		\
    int nb_iter   = (size_a+ElementsPerVector-1)/ElementsPerVector;	\
    int nb_entier = (unaligned||use_stdc) ? 0 : nb_iter*ElementsPerVector; \
    int ii = 0;								\
    /*if(nb_iter>0)*/							\
    for(ii = 0;ii<nb_entier;ii += ElementsPerVector)			\
      VECT2(Sc,adrSc,VY,ii) =  VECT2(Sa,adrSa,VY,ii) opId  VECT2(Sb,adrSb,VY,ii); \
    /*VECT(Sc,VY,ii) =  VECT(Sa,VY,ii) opId  VECT(Sb,VY,ii);*/		\
    /*VECT(Sc,VY,ii) =  _mm_div_ps(VECT(Sa,VY,ii),VECT(Sb,VY,ii));*/	\
    /*VECT(Sc,VY,ii) =  __builtin_ia32_divps(VECT(Sa,VY,ii),VECT(Sb,VY,ii));*/ \
    /*if(nb_entier<size_a)*/						\
    for(ii = nb_entier;ii<size_a;ii++ )					\
      Sc[ii] = Sa[ii] opId Sb[ii];					\
  }									\
  template <typename TX, typename VY>					\
  OP<TX,VY> operator opId (const AATV& Sa, const AATV& Sb)		\
  {									\
    OP<TX,VY> op(& funId<TX,VY>,Sa,Sb);					\
    return op;								\
  }									\
									
DefBaseOp2(add,+);
DefBaseOp2(sub,-);
DefBaseOp2(mul,*);
DefBaseOp2(div,/);
DefBaseOp2(elt_xor,^);

/**********************************************************************************/

#define DefBaseFunc1(funId,funIntrin)					\
  template <typename TX,typename VY>					\
  void funId (AATV& Sc, const AATV& Sa)					\
  {									\
    int size_a = Sa.GetSize();						\
    int size_c = Sc.GetSize();						\
    if(size_c == 0)							\
      {									\
	Sc.SetSize(size_a);						\
	size_c = size_a;						\
      }									\
    ELTWISE_BOUNDSCHECKING();						\
    TX* adrSa = Sa.bufferA;						\
    TX* adrSc = Sc.bufferA;						\
    bool unaligned = (  (((long int)adrSa)&15)			\
			||(((long int)adrSc)&15) );		\
    WarningUnaligned(unaligned);					\
    const int ElementsPerVector = sizeof(VY) / sizeof(TX);		\
    int nb_iter   = (size_a+ElementsPerVector-1)/ElementsPerVector;	\
    int nb_entier = (unaligned||use_stdc) ? 0: nb_iter*ElementsPerVector; \
    int ii = 0;								\
    /*if(nb_iter>0)*/							\
    for(ii = 0;ii<nb_entier;ii += ElementsPerVector)			\
      VECT2(Sc,adrSc,VY,ii) =  funIntrin (VECT2(Sa,adrSa,VY,ii));	\
    /*VECT(Sc,VY,ii) =  funIntrin (VECT(Sa,VY,ii));*/			\
    /*if(nb_entier<size_a)*/						\
    for(ii = nb_entier;ii<size_a;ii++ )					\
      Sc[ii] = funId(Sa[ii]);						\
  }									\
  template <typename TX,typename VY>					\
  OP<TX,VY> funId (const AATV& Sa)					\
  {									\
    OP<TX,VY> op(& funId<TX,VY>,Sa);					\
    return op;								\
  }									\
									
DefBaseFunc1(  inv            ,    _mm_rcp_ps         );			
DefBaseFunc1(  sqrt           ,    _mm_sqrt_ps        );
DefBaseFunc1(  rsqrt          ,    _mm_rsqrt_ps       );
DefBaseFunc1(  conj           ,    conj               );   //complex conjugate
DefBaseFunc1(  abs2           ,    abs2               );   //abs2 complex
DefBaseFunc1(  normcx         ,    normcx             );   //normalisation complex: a/abs(a)

/**********************************************************************************/

/**********************************************************************************/

#define DefBaseOp1(funId,opId)						\
  template <typename TX,typename VY>					\
  void funId (AATV& Sc, const AATV& Sa)					\
  {									\
    int size_a = Sa.GetSize();						\
    int size_c = Sc.GetSize();						\
    if(size_c == 0)							\
      {									\
	Sc.SetSize(size_a);						\
	size_c = size_a;						\
      }									\
    ELTWISE_1_BOUNDSCHECKING();						\
    TX* adrSa = Sa.bufferA;						\
    TX* adrSc = Sc.bufferA;						\
    bool unaligned = (  (((long int)adrSa)&15)				\
			||(((long int)adrSc)&15) );			\
    WarningUnaligned(unaligned);					\
    const int ElementsPerVector = sizeof(VY) / sizeof(TX);		\
    int nb_iter   = (size_a+ElementsPerVector-1)/ElementsPerVector;	\
    int nb_entier = (unaligned||use_stdc) ? 0: nb_iter*ElementsPerVector; \
    int ii = 0;								\
    /*if(nb_iter>0)*/							\
    for(ii = 0;ii<nb_entier;ii += ElementsPerVector)			\
      VECT2(Sc,adrSc,VY,ii) =  opId (VECT2(Sa,adrSa,VY,ii));		\
    /*VECT(Sc,VY,ii) =  opId (VECT(Sa,VY,ii));*/			\
    /*if(nb_entier<size_a)*/						\
    for(ii = nb_entier;ii<size_a;ii++ )					\
      Sc[ii] = opId(Sa[ii]);						\
  }									\
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const AATV& Sa)				\
  {									\
    OP<TX,VY> op(& funId<TX,VY>,Sa);					\
    return op;								\
  }									\
									
DefBaseOp1(opposite,-); //operator opposite

/**********************************************************************************/

#define DefBaseOpScalar(funId,opId)					\
  /*********Scalar func*********************************************/	\
  template <typename TX,typename VY>					\
  void funId (AATV& Sc, const AATV& Sa,  const TX& Scalar)		\
  {									\
    int size_a = Sa.GetSize();						\
    int size_c = Sc.GetSize();						\
    if(size_c == 0)							\
      {									\
	Sc.SetSize(size_a);						\
	size_c = size_a;						\
      }									\
    ELTWISE_1_BOUNDSCHECKING();						\
    TX* adrSa = Sa.bufferA;						\
    TX* adrSc = Sc.bufferA;						\
    bool unaligned = (  (((long int)adrSa)&15)				\
			||(((long int)adrSc)&15) );			\
    WarningUnaligned(unaligned);					\
    const int ElementsPerVector = sizeof(VY) / sizeof(TX);		\
    /*Code Not optimal here*/						\
    AATV VScalar; VScalar.SetSize(ElementsPerVector);			\
    for(int ii=0;ii<ElementsPerVector;ii++)VScalar[ii] = Scalar;	\
    /*---------------------*/						\
    int nb_iter   = (size_a+ElementsPerVector-1)/ElementsPerVector;	\
    int nb_entier = (unaligned||use_stdc) ? 0 : nb_iter*ElementsPerVector; \
    int ii = 0;								\
    /*if(nb_iter>0)*/							\
    for(ii = 0;ii<nb_entier;ii += ElementsPerVector)			\
      VECT2(Sc,adrSc,VY,ii) =   VECT(VScalar,VY,0) opId VECT2(Sa,adrSa,VY,ii) ; \
    /*VECT(Sc,VY,ii) =   VECT(VScalar,VY,0) opId VECT(Sa,VY,ii) ;*/	\
    /*if(nb_entier<size_a)*/						\
    for(ii = nb_entier;ii<size_a;ii++ )					\
      Sc[ii] = (Scalar) opId (Sa[ii]) ;					\
  }									\
  /*********Scalar on OPA*********************************************/ \
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const TX& Scalar, const AATV& Sb)		\
  {									\
    OP<TX,VY> op(& funId<TX,VY>,Sb,Scalar);				\
    return op;								\
  }									\
  
#define DefBaseOpScalarOPB(funId,opId)					\
  /*********Scalar on OPB*********************************************/	\
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const AATV& Sa,const TX& Scalar)		\
  {									\
    OP<TX,VY> op(& funId<TX,VY>,Sa,Scalar);				\
    return op;								\
  }									\

DefBaseOpScalar(add_scalar,+);DefBaseOpScalarOPB(add_scalar,+);
DefBaseOpScalar(sub_scalar,-);DefBaseOpScalarOPB(sub_scalar,-);
DefBaseOpScalar(mul_scalar,*);DefBaseOpScalarOPB(mul_scalar,*);
DefBaseOpScalar(div_scalar,/);//Scalar on OPB is not allowed for division
								 
template <typename TX,typename VY>				 
OP<TX,VY> operator / (const AATV& Sa,const TX& Scalar)	 
{		
  cout<<c_red<<"Error: Sig/Scalar is not allowed with libsig"<<c_normal<<endl;						 
  myabort();
  OP<TX,VY> op(& div_scalar<TX,VY>,Sa,Scalar);	
  return op;
} 

/**********************************************************************************/

#define DefBaseOp_OPOP(funId,opId)					\
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const OP<TX,VY>& Op1, const OP<TX,VY>& Op2)	\
  {									\
    AATV Temp1; Temp1 = Op1;/*Op1.exec(Temp1);*/			\
    AATV Temp2; Temp2 = Op2;/*Op2.exec(Temp2);*/			\
    OP<TX,VY> op(& funId<TX,VY>,Temp1,Temp2);				\
    return op;								\
  }									\
									
DefBaseOp_OPOP(add,+);
DefBaseOp_OPOP(sub,-);
DefBaseOp_OPOP(mul,*);
DefBaseOp_OPOP(div,/);

/**********************************************************************************/

#define DefBaseOp_OPSIG(funId,opId)					\
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const OP<TX,VY>& Op1, const AATV& Sig2)	\
  {									\
    AATV Temp1; Temp1 = Op1;/*Op1.exec(Temp1);*/			\
    OP<TX,VY> op(& funId<TX,VY>,Temp1,Sig2);				\
    return op;								\
  }									\
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const AATV& Sig1, const OP<TX,VY>& Op2)	\
  {									\
    AATV Temp2; Temp2 = Op2;/*Op2.exec(Temp2);*/			\
    OP<TX,VY> op(& funId<TX,VY>,Sig1,Temp2);				\
    return op;								\
  }									\
									
DefBaseOp_OPSIG(add,+);
DefBaseOp_OPSIG(sub,-);
DefBaseOp_OPSIG(mul,*);
DefBaseOp_OPSIG(div,/);

/**********************************************************************************/

#define DefBaseOp_OPSCA(funId,opId)					\
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const OP<TX,VY>& Op1, const TX& ScalarVal)	\
  {									\
    AATV Temp1; Temp1 = Op1;/*Op1.exec(Temp1);*/			\
    OP<TX,VY> op(& funId<TX,VY>,Temp1,ScalarVal);			\
    return op;								\
  }									\
  template <typename TX,typename VY>					\
  OP<TX,VY> operator opId (const TX& ScalarVal, const OP<TX,VY>& Op2)	\
  {									\
    AATV Temp2; Temp2 = Op2;/*Op2.exec(Temp2);*/			\
    OP<TX,VY> op(& funId<TX,VY>,Temp2,ScalarVal);			\
    return op;								\
  }									\
									
DefBaseOp_OPSCA(add_scalar,+);
DefBaseOp_OPSCA(sub_scalar,-);
DefBaseOp_OPSCA(mul_scalar,*);
DefBaseOp_OPSCA(div_scalar,/);



/**************Functions Prototype*******************************/
//Sigcxs * float
OP<Cxs,__v2Cxs> operator * (const Sigcxs &Sin, const float &alpha);
void mul_scalar_sigcxs_float(Sigcxs &Sout, const Sigcxs &Sin, const Cxs &alpha);

#endif
