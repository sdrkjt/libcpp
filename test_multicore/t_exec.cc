#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  char *args[4];
  args[0] = ("ls"  );
  args[1] = ("-l"  );
  args[2] = (".");
  args[3] = NULL;
  printf("START TESTING!!");
  getchar();
  execv("/bin/ls",args);
  getchar();
  printf("WELL DONE!!");
  //exit(EXIT_SUCCESS);
}
