#pragma once

class PageAllocator{
    public:
    PageAllocator();
    void* GetProtectablePage();
    ~PageAllocator();

    private:
    void *allocated_start;
};