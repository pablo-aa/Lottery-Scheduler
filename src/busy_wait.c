#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

/**
    * This file is responsible to execute the busy wait process.
    * 
    * @file busy_wait.c
*/

int main(){
    if(VERBOSE){
        printf("Starting busy_wait.c\n");
    }
    // Process with time around 30 seconds
    for(long i = 0; i < 23000000000; i++){}
    return 0;
}