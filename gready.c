#include "stdio.h"
#include "stdlib.h"

int main(int argc, char *argv[]) {

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    int n;
    fscanf(fp, "%d", &n);
    

    fclose(fp);
    return 0;
}