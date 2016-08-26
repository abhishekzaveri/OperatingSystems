/* 
 
 Ittai Shay & Abhishek Zaveri

 Problem 1: Write a program pipe_test which creates three processes, P1, P2, P3 and pipes as needed. 
 The first process displays its pid, prompts the user for a string and then sends it to the second process. 
 The second process displays its pid, displays the string it received, reverses the string, displays the 
 reversed string, and sends it to the third process. The third process displays its pid, displays the 
 string it received, converts the string to uppercase, displays the string the string it generated, and 
 sends it back to the first process. When the first process gets the processed string, it displays its pid 
 and displays the string it received it to the terminal. When all this is done, all processes terminate but 
 display their pid and a message announcing their imminent death.

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER 80

/* reverse a char array */
void reverse(char *str) {
   size_t start = 0, end = strlen(str) - 1;
   while (start < end) {
      char temp = str[start];
      str[start] = str[end];
      str[end] = temp;
      start++;
      end--;
   }
}

/* capitalize every element in a char array */
void capitalize(char *str) {
   size_t len = strlen(str);
   for (int i = 0; i < len; i++) {
      if (isalpha(str[i])) {
         str[i] = toupper(str[i]);
      }
   }
}

/* print process name and pid */
void printPID(char *process) {
   printf("\nI am process %s\nMy pid is %d\n", process, getpid());
}

/* print process name and pid and "die message" */
void printDieMessage(char *process) {
   printPID(process);
   printf("I am about to die\n");
}

/* print string to send */
void printSendString(char *str, char *dst) {
   printf("I am sending %s to %s\n", str, dst);
}

/* print string received */
void printStringReceived(char *str) {
   printf("I just received the string %s\n", str);
}

int main(int argc, const char * argv[]) {
   int fd[2];
   pipe(&fd[0]);

   char *p1 = "P1", *p2 = "P2", *p3 = "P3";
   char readbuffer[BUFFER];

   /* P1 retrieves a string from the user */
   printPID(p1);
   printf("Please enter a string: ");
   scanf("%s", readbuffer);

   /* P1 writes to the pipe */
   write(fd[1], readbuffer, (strlen(readbuffer)+1));

   if (fork() == 0) {
      /* P2 reverses the string */
      printPID(p2);
      read(fd[0], readbuffer, sizeof(readbuffer));
      printStringReceived(readbuffer);
      reverse(readbuffer);
      printSendString(readbuffer, p3);
      write(fd[1], readbuffer, (strlen(readbuffer)+1));

      if (fork() == 0) {
         /* P3 capitalizes the string */
         printPID(p3);
         read(fd[0], readbuffer, sizeof(readbuffer));
         close(fd[0]); /* input side of pipe no longer needed */
         printStringReceived(readbuffer);
         capitalize(readbuffer);
         printSendString(readbuffer, p1);
         write(fd[1], readbuffer, (strlen(readbuffer)+1));
         close(fd[1]); /* output side of pipe no longer needed */
         printDieMessage(p3);
      } else {
         wait(NULL);
         close(fd[0]); /* input side of pipe no longer needed */
         close(fd[1]); /* output side of pipe no longer needed */
         printDieMessage(p2);
      }
   } else {
      wait(NULL);
      close(fd[1]); /* output side of pipe no longer needed */

      /* P1 displays the final result */
      printPID(p1);
      read(fd[0], readbuffer, sizeof(readbuffer));
      close(fd[0]); /* input side of pipe no longer needed */
      printStringReceived(readbuffer);
      printDieMessage(p1);
   }

   return 0;
}
