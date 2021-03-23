#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("USER = %s\n", getenv("USER"));
    return 0;
}