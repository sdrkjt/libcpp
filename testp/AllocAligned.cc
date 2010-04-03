/****************2 METHODs for Memory Alignement************************/

char * AllocMem(int numElt){
  char * ptr = new char[numElt + 16];
  int offset = (int)ptr & 0xf;
  if(offset)
    ptr = (char*)( (unsigned int)ptr + 16 - offset );
  return  ptr;
};

#define AlignBy 16

float * AllocMemSig(int num){
  char * buffer2U = 0;                          // New buffer, unaligned
  // Aligned pointer to new buffer:
  union {
    char * b;                                  // Used for converting from char*
    float * p;                                    // Converted to TX *
    long int i;                                // Used for alignment
  } buffer2A;
   
  buffer2U = new char[num*sizeof(float)+AlignBy-1];// Allocate new buffer

  buffer2A.b = buffer2U + AlignBy - 1;
   
  buffer2A.i &= - (long int)AlignBy;
 
  return buffer2A.p;
};
/************************************************************************/
