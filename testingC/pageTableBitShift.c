#include <stdio.h>

int main(void)
{
    // the PKU bits in PTE are 62:59 
    unsigned long long shiftBy = 59;
    for (unsigned long long currentKey = 0; currentKey < 16; currentKey++)
    {
        unsigned long long shiftedKey = currentKey << shiftBy;
        printf("shifted value of key '%llu' is '%llu'\n", currentKey, shiftedKey);
    }
}