#include "../../include/timer.h"
#include <stdio.h>
#include <l4/util/rdtsc.h>

void MPKTimer::TimeNow(PointInTime *writeTo)
{
    // asm volatile("MFENCE\n\t LFENCE" :::);
    asm volatile("MOV $0, %%eax\n\t CPUID" ::: "eax", "ebx", "ecx", "edx");

    writeTo->duration = l4_rdtsc();
    // writeTo->core_cycles_not_halted = l4_rdpmc((1 << 30) + 1);
    // writeTo->reference_cycles_not_halted = l4_rdpmc((1 << 30) + 2);

    asm volatile("MOV $0, %%eax\n\t CPUID" ::: "eax", "ebx", "ecx", "edx");
    // asm volatile("LFENCE" :::);
}

void MPKTimer::Start()
{
    TimeNow(&StartPoint);
}

MPKTimer::DurationMPK MPKTimer::Stop(unsigned int test_iteration)
{
    TimeNow(&EndPoint);
    DurationMPK duration;
    duration.duration = EndPoint.duration - StartPoint.duration;
    // duration.core_cycles_not_halted = EndPoint.core_cycles_not_halted - StartPoint.core_cycles_not_halted;
    // duration.reference_cycles_not_halted = EndPoint.reference_cycles_not_halted - StartPoint.reference_cycles_not_halted;
    Results[test_iteration] = duration;
    return duration;
}

std::vector<char> MPKTimer::ResultsForExport(char seperator_values, char seperator_lines)
{
    const int intro_text_length = 50;   
    const int max_expected_line_length = 14; // tsc duration => 10 max + (cycles not halted => 10 * 2) + seperatorchars => 4
    // even though
    unsigned int max_length = amount_of_results * max_expected_line_length + intro_text_length;
    std::vector<char> returnValue = std::vector<char>(max_length);
    
    unsigned int current_write_index = 0;
    {
        int written_characters = sprintf(&returnValue[0], "--RegexStartCSVMarker--durationTSC%c", seperator_lines);
        current_write_index += written_characters;
    }
    for (DurationMPK &element : Results)
    {
        int written_characters = snprintf(&returnValue[current_write_index], 
        max_length - current_write_index, 
        "%llu%c", 
        element.duration, seperator_lines);
        current_write_index += written_characters;
    }
    snprintf(&returnValue[current_write_index], max_length - current_write_index, "--RegexEndCSVMarker--");

    return returnValue;
}