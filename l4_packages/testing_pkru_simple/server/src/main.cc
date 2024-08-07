
// #include <../../../mpklibrary/include/pkrulib.h> // for development
// #include <../../../timingMPK/include/timer.h>
#include <l4/mpklibrary/pkrulib.h>
#include <l4/timingMPK/timer.h>
#include <stdio.h>


void SimplePKRUWrite(MPKTimer *timer, int amount_of_results);

int main(void)
{
  printf("Simple Testing PKRU says Hello\n");

  // Calling Setup of Timer
  const int amount_of_results = 1000;
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  SimplePKRUWrite(&timer, amount_of_results);
  // Testing
  SimplePKRUWrite(&timer, amount_of_results);  

  // Results
  auto results = timer.ResultsForExport(',', ';');
  puts("Results of Simple Testing PKRU are:");
  puts(results);

  return 0;
}



void SimplePKRUWrite(MPKTimer *timer, int amount_of_results)
{
  unsigned int current_val = 0;

  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      // technically it should be before timer start but the loop variant has to have it here so we take it here aswell
      current_val += 4; // never associate key 0
      PKRUlib::write(current_val);
    }
    timer->Stop(i);
  }
}