#include "../../include/protectablepage.h"
#include <stdlib.h>
#include <cstring>

PageAllocator::PageAllocator()
{    
    allocated_start = malloc(4 * L4_PAGESIZE);
    memset(allocated_start, 0x12, 4 * L4_PAGESIZE);
}

void *PageAllocator::GetProtectablePage()
{
    return static_cast<char*>(allocated_start) + L4_PAGESIZE; // + some offset
}

PageAllocator::~PageAllocator()
{
    free(allocated_start);
}