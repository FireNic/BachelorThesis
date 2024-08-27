#pragma once
#include <vector>
class MPKTimer
{
public:
    typedef unsigned long long DurationTSC;
    struct DurationMPK{
        DurationTSC duration;
        // unsigned long long core_cycles_not_halted;
        // unsigned long long reference_cycles_not_halted;
    };

    MPKTimer(unsigned int amount_results) {
        Results = std::vector<DurationMPK>(amount_results);
        amount_of_results = amount_results;
    }
    void Start();
    DurationMPK Stop(unsigned int test_iteration);
    std::vector<char> ResultsForExport(char seperator_values, char seperator_lines);
    

private:
    typedef DurationMPK PointInTime;
    unsigned int amount_of_results;
    PointInTime StartPoint;
    PointInTime EndPoint;
    std::vector<DurationMPK> Results;
    void TimeNow(PointInTime *writeTo);
};