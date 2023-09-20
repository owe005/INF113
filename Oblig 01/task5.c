#include <stdio.h>
#include <stdlib.h>

int main(){
   int *data = (int *)malloc(sizeof(int) * 100);
   free(&data[5]);
   printf("%c", data[0]);
   
}