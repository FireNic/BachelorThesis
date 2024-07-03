#define _GNU_SOURCE
#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <cpuid.h>
#include <stdlib.h>
#include <sys/mman.h>

extern unsigned int testCPUID(void);

int main(void)
{
    int extendedFeaturesECXRegister = testCPUID();
    printf("testCPUID finished with %u\n", extendedFeaturesECXRegister);
    int pkuMask =     0b00000000000000000000000000001000;
    int ospkuMask =   0b00000000000000000000000000010000;

    if((extendedFeaturesECXRegister & pkuMask) == 0b1000){
        printf("pku feature exists\n");
    }
    else{
        printf("pku feature does not exist\n");
    }

    if((extendedFeaturesECXRegister & ospkuMask) == 0b10000){
        printf("pku feature is enabled\n");
    }
    else{
        printf("pku feature is not enabled\n");
    }

    printf("\n\n----------------\n\n");

    return 0;
}