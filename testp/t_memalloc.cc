#include <emmintrin.h>
#include <math.h>
#include "../libsig/libsig.hh"

int main(){
  /*pay attention with option O3 when testing
   *the delete array function works with an adr value
   *ex: int* data1; data1 = new int[1000]
   *    int* data2; data2 = data1; init_exec(data2);
   *    delete [] data2;//OK
   */
    int ** tab;
    int nbrow = 25600, nbcol = 6400;

    mygetchar();
    tab = memalloc<int>(nbrow,nbcol);
    
    for(int ii=0;ii<nbrow;ii++){
      for(int jj=0;jj<nbcol;jj++){
	*(*(tab+ii)+jj) =  (ii*nbcol+jj);
      }
    }
    cout<<"TEST TAB INIT\n";
    int sum = 0;
    for(int ii=0;ii<nbrow;ii++)
      for(int jj=0;jj<nbcol;jj++)
	sum += (int)(tab[ii][jj])/nbrow/nbcol;
    DUMP(sum);

    mygetchar();
    memfree(tab);
    mygetchar();

    
  return 1;
}
