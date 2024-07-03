#define _GNU_SOURCE
#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <cpuid.h>
#include <stdlib.h>
#include <sys/mman.h>

extern int testCPUID();

int main(void)
{
    int extendedFeaturesECXRegister = testCPUID();
    printf("testCPUID finished with %d\n", extendedFeaturesECXRegister);
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

    unsigned int eax, ebc, ecx, edx;
    eax = 0;
    ecx = 0;
    __get_cpuid(7, &eax, &ebc, &ecx, &edx);
    printf("eax %d\nebc %d\necx %d\nedx %d\n", eax, ebc, ecx, edx);
    return 0;
}