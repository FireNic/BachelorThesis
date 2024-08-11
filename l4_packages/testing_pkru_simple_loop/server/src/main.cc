
// #include <../../../mpklibrary/include/pkrulib.h> // for development
// #include <../../../timingMPK/include/timer.h>
#include <l4/mpklibrary/pkrulib.h>
#include <l4/timingMPK/timer.h>
#include <stdio.h>


void SimplePKRUWrite(MPKTimer *timer, int amount_of_results, int inner_loop_count);

int main(void)
{
  printf("Simple Testing PKRU Loop says Hello\n");

  // Calling Setup of Timer
  const int amount_of_results = 1000;
  const int amount_of_inner_loops = 1000;
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  SimplePKRUWrite(&timer, amount_of_results, amount_of_inner_loops);
  // Testing
  SimplePKRUWrite(&timer, amount_of_results, amount_of_inner_loops);  

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  printf("Results of Simple Testing PKRU Loop (count per time = %d) are:\n", amount_of_inner_loops);
  puts(&results[0]);

  return 0;
}



void SimplePKRUWrite(MPKTimer *timer, int amount_of_results, int inner_loop_count)
{
  unsigned int current_val = 0;
  // Outside Loop for averaging Results
  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      // Single Testing (Either One Instruction or a Loop)
      for(int j = 0; j < inner_loop_count; j++)
      {
        current_val += 4; // never associate key 0
        PKRUlib::write(current_val);
      }
      // End Of Single Test
    }
    timer->Stop(i);
  }
}