/*
 
 Ittai Shay & Abhishek Zaveri
 
 Problem 4: Write a program process_tree that takes a single integer argument 
 n from the command line and creates a binary tree of processes of depth n. 
 When the tree is created, each process, each process should display the phrase
 I am process x; my process pid is p (the pid for process x); My parents pid 
 is p1 (the pid for this process’ parent)
 After printing as indicated above, the process terminates.
 
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

/* forks the binary tree recursively */
void recursivelyFork(int totalDepth, int currentDepth, int counter) {

   /* print process information */
   printf("\nI am process %d; My process pid is %d; My parents pid is %d\n",
          counter, getpid(), getppid());

   /* stop forking at desired tree depth */
   if (currentDepth == totalDepth) {
      return;
   }

   /* each process forks two new processes */
   for (int i = 0; i < 2; i++) {
      if (!fork()) {
         /* recursively fork from the child */
         recursivelyFork(totalDepth, currentDepth + 1, (counter * 2) + i);
         break;
      } else {
         /* wait until all child processes are terminated */
         wait(NULL);
      }
   }
}

int main(int argc, const char * argv[]) {
   if (argc != 2) {
      printf("One argument (tree depth) expected.\n");
   } else {
      recursivelyFork(atoi(argv[1]), 1, 1);
   }

   return 0;
}
