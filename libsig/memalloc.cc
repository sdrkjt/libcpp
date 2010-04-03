#include "memalloc.hh"

int64          sta::pAdr          = 0 ;
int64          sta::clock         = 0 ;
vector<ida*>   sta::lmember           ;
//char           sta::MEM[MAX_MEM_SIZE] ;
char*           sta::MEM = NULL;
