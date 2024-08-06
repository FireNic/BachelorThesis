#pragma once
#include <vector>
#include <chrono>
class PKRUTimer
{
public:
    typedef unsigned long long DurationTSC;

    PKRUTimer(unsigned int amount_results) {
        Results = std::vector<DurationTSC, std::allocator<DurationTSC>>(amount_results);
        amount_of_results = amount_results;
    }
    void Start();
    DurationTSC Stop(unsigned int test_iteration);
    char* ResultsForExport(char seperator);
    

private:
    typedef unsigned long long PointInTime;
    unsigned int amount_of_results;
    PointInTime StartPoint;
    PointInTime EndPoint;
    std::vector<DurationTSC, std::allocator<DurationTSC>> Results;
    void TimeNow(PointInTime *writeTo);
};