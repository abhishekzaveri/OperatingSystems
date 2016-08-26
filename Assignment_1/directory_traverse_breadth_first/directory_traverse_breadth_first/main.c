/*
 
 Ittai Shay & Abhishek Zaveri
 
 Problem 3: Write a program directory_traverse_breadth_first that takes as argument a path 
 to a directory searching for a file name. To convince us that your program is working as 
 intended, print out the directory and file names it encounters in the search.
 
 */

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

struct Node {
   char* address; /* address of a directory */
   struct Node* next; /* pointer to the next directory in the queue */
};

struct Node *first = NULL, *last = NULL, *temp = NULL; /* queue management */
char rootaddress[255], filename[255]; /* hold the arguments in global variables */

void enqueue(char* address) {
   char* fulladdress = (char*) malloc(255*sizeof(char));

   if (last == NULL) { /* queue is empty */
      last = (struct Node *) malloc (1*sizeof(struct Node));
      last->next = NULL;
      last->address = strcpy(fulladdress, address);
      first = last;
   } else {
      temp = (struct Node *) malloc (1*sizeof(struct Node));
      last->next = temp;
      temp->address = strcpy(fulladdress, address);
      temp->next = NULL;

      last = temp;
   }
}

char* dequeue() {
   temp = first;
   if (temp == NULL) { /* queue is empty, return null */
      return NULL;
   }

   char* filepath = temp->address;

   if (temp->next != NULL) {
      temp = temp->next;
      free(first);
      first = temp;
   } else {
      free(first);
      first = NULL;
      last = NULL;
   }

   return filepath;
}

 /* traverse the directories via BFS */
void traverse() {
   struct dirent *item;

   while (first != NULL) { /* loop until the queue is empty */

      DIR dir = *opendir(first->address);
      while((item = readdir(&dir)) != NULL) { /* checks every item in the directory */

         /* ignore "." and ".." folders */
         if (!(strcmp(item->d_name, "..") && strcmp(item->d_name, "."))) {
            continue;
         }

         if (item->d_type & DT_DIR) { /* item is a directory */
            char path[255];
            strcpy(path, first->address);
            strcat(path, "/");
            strcat(path, item->d_name);
            enqueue(path);
            printf("            %s\n", item->d_name);
         } else { /* item is a file */
            if (!strcmp(item->d_name, filename)) {
               printf("File Found! ");
            } else {
               printf("            "); // inverses 
            }
            printf("%s\n", item->d_name);
         }
      }

      dequeue();
   }
}

/* ensures the root directory path is absolute */
void makePathAbsolute(char* dir){
   if (dir[0] == '/') return; /* dir is already absolute */

   char absolute[255], cwd[255];
   getcwd(cwd, 255); /* get current working directory */

   /* absolute path is "cwd/dir" */
   strcpy(absolute, cwd);
   strcat(absolute, "/");
   strcat(absolute, dir);
   strcpy(dir, absolute);
}

int main(int argc, const char * argv[]) {
   if (argc != 3) {
      printf("Two arguments (root directory and desired file name) expected.\n");
      return 0;
   }

   strcpy(rootaddress, argv[1]);
   strcpy(filename, argv[2]);

   makePathAbsolute(rootaddress);

   /* check that root address is valid */
   DIR *dir = opendir(rootaddress);
   if (dir == NULL) {
      printf("Invalid root directory.\n");
      return 0;
   } else {
      closedir(dir);
   }

   enqueue(rootaddress);
   traverse();

   return 0;
}
