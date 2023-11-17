#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Process with time around 30 seconds
int main(){
    printf("Starting busy_wait.c\n");
    // time_t start, end;
    // double elapsed;
    // time(&start);
    for(long i = 0; i < 24000000000; i++){}
    // time(&end);
    // elapsed = difftime(end, start);
    // printf("Time elapsed: %f\n", elapsed);
    return 0;
}