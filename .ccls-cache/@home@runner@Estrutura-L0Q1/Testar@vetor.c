#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHAR 1000

int main(){
  char v[1000];
  int i, cont = 0;

  for(i = 0; i < MAX_CHAR; i++)
    v[i] = 'a';
  
  for(i = 0; i < MAX_CHAR; i++)
    printf("%c", v[i]);

  for(i = 0; i < MAX_CHAR; i++)
    if(v[i] == 'a') cont++;
  printf("\nNúmero de letras: %d", cont);
  return 0;
}