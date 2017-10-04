#ifndef SORT_H
#define SORT_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node
{
   char *data;
   struct node *next;
} node;

char *readline (FILE *fp, char **buffer);
struct node* insertInAscOrder(char *fileName, struct node *head);
void printList(struct node* head);
void freeNode(struct node* head);
int isInt(char* input);


#endif

