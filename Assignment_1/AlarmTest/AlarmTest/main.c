/*
 
 Ittai Shay & Abhishek Zaveri
 
 Problem 2. Write a program AlrmTest that does the following. Creates two child processes 
 that each increments a process-local variable count in an infinite loop. The parent is 
 just a for loop of 5 iterations, where in each interation it sleeps for a second and 
 then sends the SIGALRM signal to each child. Upon receiving this signal, each child is 
 to print out the value of count along with its process id and reset the count to zero.
 
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int count = 0, seconds = 0;

/* print the count, reset count, increment seconds */
void handle_alarm() {
   printf("Count is %11d, my pid is %d\n", count, getpid());
   count = 0;
}

/* increment counter in an infinite loop */
void increment() {
   while (seconds < 5) {
      count++;
   }
}

int main(int argc, const char * argv[]) {
   pid_t child_a, child_b;
   static struct sigaction action;
   action.sa_handler = handle_alarm;

   if (!(child_a = fork())) {
      sigaction(SIGALRM, &action, NULL); /* register handler in child process */
      increment();
   } else {
      if (!(child_b = fork())) {
         sigaction(SIGALRM, &action, NULL); /* register handler in child process */
         increment();
      } else {
         for (int i = 0; i < 5; i++) {
            /* wait for a second, then signal the children */
            sleep(1);
            kill(child_a, SIGALRM);
            kill(child_b, SIGALRM);
         }
         kill(child_a, SIGKILL);
         kill(child_b, SIGKILL);
         wait(NULL);
      }
   }
   return 0;
}
