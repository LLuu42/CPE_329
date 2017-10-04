#include "wordsort.h"

int main(int argc, char * argv[])
{
   struct node *head = NULL;
   char *line = NULL;
   int listLen;
   int i;
   printf("How many items are going to be in your list?\n");
   line = readline(stdin, &line);

   if(!isInt(line))
   {
      printf("Invalid Entry. Exiting program.\n");
      return 1;
   }

   listLen = atoi(line);
   free(line);

   printf("\n");
   printf("Enter the %d entries now:\n", listLen);

   for(i = 0; i < listLen; ++i)
   {
      line = readline(stdin, &line);
      head = insertInAscOrder(line, head);
      free(line);
   }

   printf("\n");
   printList(head);
   return 0;
}

int isInt(char* input)
{
   int i = 0;
   while(i < strlen(input))
   {
      if(!isdigit(*(input + i)))
      {
         return 0;
      }
      ++i;
   }
   return 1;
}

void printList(struct node* head)
{
   struct node *tmp;
   printf("Sorted List:\n");
   while(head != NULL)
   {
      tmp = head;
      printf("%s\n", head->data);
      head = head->next;
      freeNode(tmp);
   }
}

void freeNode(struct node* head)
{
   free(head->data);
   free(head);
}

char *readline (FILE *fp, char **buffer)
{
   int ch;
   size_t buflen = 0,
   nchar = 10;

   *buffer = malloc (nchar);
   if (*buffer == NULL)
   {
      fprintf (stderr, "Not enough memory space. ): \n");
      return NULL;
   }

   while ((ch = fgetc(fp)) != '\n' && ch != EOF)
   {
       (*buffer)[buflen++] = ch;

        if (buflen + 1 >= nchar)
        {
           char *tmp = realloc (*buffer, nchar * 2);
           if (!tmp)
           {
               fprintf (stderr, "reallocate failed ): \n");
               (*buffer)[buflen] = 0;
               return *buffer;
           }
           *buffer = tmp;
           nchar *= 2;
       }
   }

   (*buffer)[buflen] = '\0';
   if (ch == EOF)
   {
      free (*buffer);
      *buffer = NULL;
   }

   return *buffer;
}


struct node* insertInAscOrder(char *data, struct node *head)
{
   struct node *newNode;
   struct node *tmp;
   struct node *previous;

   newNode = (struct node*) malloc(sizeof(node));

   if(newNode == NULL) //check to see if node is created
   {
      printf("Insufficient memory.\n");
      exit(-1);
   }
   newNode->data = strdup(data);
   newNode->next = NULL;

   if(head == NULL)
   {
      head = newNode;
   }
   else if(strcmp(data, head->data) < 0)
   {
      newNode->next = head;
      head = newNode;
   }
   else
   {
      previous = head;
      tmp = head->next;
      while(tmp != NULL && (strcmp(data, tmp->data) > 0))
      {
         previous = tmp;
         tmp = tmp->next;
      }
      if(tmp == NULL)
      {
         previous->next = newNode;
      }
      else
      {
         newNode->next = tmp;
         previous->next = newNode;
      }
   }
   return head;
}

