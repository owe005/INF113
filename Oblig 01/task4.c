#include <stdio.h>
#include <stdlib.h>

int main(){
   int *data = (int *)malloc(sizeof(int) * 100);
   free(data);
   printf("%c", data[0]);
}