
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int main ()
{
   int i;
    
   /* 3 calls/messages to file system */
   close(0);
   close(1);
   close(2);

   /* 2*10 calls/messages to process manager */
   for (i = 0; i < 10; i++) {
      if (!fork()) return 0;
      alarm(0);
   }

   pause();
   return 0;
}