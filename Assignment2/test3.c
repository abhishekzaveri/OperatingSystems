
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int main ()
{
   int i;
   for (i = 0; i < 100; i++) {
      getpid(); /* 20 */
      if (i > 5) continue;
      getuid(); /* 24 */
      getgid(); /* 47 */
   }
   pause(); /* 29 */
   return 0;
}