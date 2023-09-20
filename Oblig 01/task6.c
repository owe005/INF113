#include <stdio.h>
#include <stdlib.h>

void silly_add(int *a, int *b) {
    if (*b == 0) return;
    *a += 1;
    *b -= 1;
    silly_add(a, b);
}

int main(int argc, char** argv) {
    if (argc != 3) { 
        printf("Error, input two positive numbers\n");
        return 0; 
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    silly_add(&a, &b);
    printf("%d\n", a);
    return 0;
}