
// #include <../../../mpklibrary/include/pkrulib.h> // for development
// #include <../../../timingMPK/include/timer.h>
#include <l4/mpklibrary/pkrulib.h>
#include <l4/timingMPK/timer.h>

#include <l4/re/env>
#include <l4/sys/segment.h>

#include <stdlib.h>
#include <stdio.h>


void SimpleSyscall(MPKTimer *timer, int amount_of_results, unsigned int *to_touch);

int main(void)
{
  printf("Simple Testing Syscall says Hello\n");

  const int amount_of_results = 1000;

  unsigned int *touching_this_memory = static_cast<unsigned int*>(malloc(sizeof(*touching_this_memory)));
  *touching_this_memory = 20;
  PKRUlib::write(~0b11); // disable everything except key 0

  // Calling Setup of Timer
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  SimpleSyscall(&timer, amount_of_results, touching_this_memory);
  // Testing
  SimpleSyscall(&timer, amount_of_results, touching_this_memory);  

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  puts("Results of Simple Testing Syscall are:");
  puts(&results[0]);

  return 0;
}



void SimpleSyscall(MPKTimer *timer, int amount_of_results, unsigned int *to_touch)
{
  auto utcb = l4_utcb();
  auto cap = L4Re::Env::env()->task().cap();

  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      fiasco_pku_set(cap, i % 16, to_touch, utcb);
    }
    timer->Stop(i);
  }
}