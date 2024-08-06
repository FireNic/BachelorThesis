#include "../../include/timer.h"
#include <stdio.h>
#include <l4/util/rdtsc.h>

void PKRUTimer::TimeNow(PointInTime *writeTo)
{
    *writeTo = l4_rdtsc();
}

void PKRUTimer::Start()
{
    TimeNow(&StartPoint);
}

PKRUTimer::DurationTSC PKRUTimer::Stop(unsigned int test_iteration)
{
    TimeNow(&EndPoint);
    DurationTSC duration = EndPoint - StartPoint;
    Results[test_iteration] = duration;
    return duration;
}

char *PKRUTimer::ResultsForExport(char seperator)
{
    const int number_of_digits_in_64_bit = 22; // its 20 but to be save
    unsigned int max_length = amount_of_results * number_of_digits_in_64_bit + amount_of_results;
    char *returnValue = new char[max_length];
    unsigned int current_write_index = 0;

    for (DurationTSC &element : Results)
    {
        int written_characters = snprintf(&returnValue[current_write_index], max_length - current_write_index, "%llu%c", element, seperator);
        current_write_index += written_characters;
    }

    return returnValue;
}