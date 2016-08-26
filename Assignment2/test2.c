
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int main ()
{
   int i;
   for (i = 0; i < 5; i++) {
      sleep(2);
      if (fork() == 0) {
         while (1) {} /* infinite loop for children */
         break;
      } else {
         sleep(2); /* sleep two seconds before forking again */
      }
   }
   pause();
   return 0;
}