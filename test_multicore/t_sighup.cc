#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



 // Action effectuée lors de la réception du signal
void action_signal (int num) {
  if (num == SIGHUP) {
    printf("Signal SIGHUP reçu\n");
    exit(EXIT_SUCCESS);
  }
 }
int main(int argc, char *argv[])
{
  struct sigaction action;
  // Fonction action_signal sera appelée
  action.sa_handler = action_signal;
  // Aucun signal n'est ignoré
  sigemptyset(&action.sa_mask);
  // Compatibilité BSD
  action.sa_flags=SA_RESTART ;
  // Modifie l'action pour le signal SIGHUP
  if(sigaction(SIGHUP,&action, NULL)==-1)
    {
      printf("Impossible d'appeler sigaction\n");
      exit(EXIT_FAILURE);
    }
  while(1) {
    printf(".\n");
    sleep(1);
  }
}
