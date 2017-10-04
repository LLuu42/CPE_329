#include <stdio.h>

#define MAX_COUNT 64
#define NUM_BITS 6

void print_header();
void digit_to_binary(unsigned int digit, int numBits, char * out);
void print_arr(char * out, int numBits);

int main()
{
   int i;
   char out[NUM_BITS + 1];// = {'a', 'b', 'c', 'd', 'e', 'f'};
   print_header();

   for(i = 0; i < MAX_COUNT; ++i)
   {
      digit_to_binary(i, NUM_BITS, out);
      print_arr(out, NUM_BITS);
   }
   return 0;
}

void print_header()
{
   printf("D5 D4 D3 D2 D1 D0\n");
   printf("-----------------\n");
}

void digit_to_binary(unsigned int digit, int numBits, char * out)
{
   
   out[NUM_BITS] = '\0';
   int mask = MAX_COUNT;
   while(mask >>= 1)
      *out++ = !!(mask & digit) + '0';
}

void print_arr(char * out, int numBits)
{
   int i;
   for (i = 0; i < numBits; ++i)
   {
      printf("%c  ", out[i]);
   }
   printf("\n");
}
