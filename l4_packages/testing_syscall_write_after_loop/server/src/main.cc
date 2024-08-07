
// #include <../../../mpklibrary/include/pkrulib.h> // for development
// #include <../../../timingMPK/include/timer.h>
#include <l4/mpklibrary/pkrulib.h>
#include <l4/timingMPK/timer.h>

#include <l4/re/env>
#include <l4/sys/segment.h>

#include <stdlib.h>
#include <stdio.h>

void SimpleSyscallWriteLoop(MPKTimer *timer, int amount_of_results, int inner_loop_count, unsigned int *to_touch);

int main(void)
{
  printf("Simple Testing Syscall Write Loop says Hello\n");

  const int amount_of_results = 1000;
  const int inner_loop_count = 1000;

  unsigned int *touching_this_memory = static_cast<unsigned int *>(malloc(sizeof(*touching_this_memory)));
  *touching_this_memory = 20;
  PKRUlib::write(~0b11); // disable everything except key 0

  // Calling Setup of Timer
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  SimpleSyscallWriteLoop(&timer, amount_of_results, inner_loop_count, touching_this_memory);
  // Testing
  SimpleSyscallWriteLoop(&timer, amount_of_results, inner_loop_count, touching_this_memory);

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  puts("Results of Simple Testing Syscall Write Loop are:");
  puts(&results[0]);

  return 0;
}

void SimpleSyscallWriteLoop(MPKTimer *timer, int amount_of_results, int inner_loop_count, unsigned int *to_touch)
{
  auto utcb = l4_utcb();
  auto cap = L4Re::Env::env()->task().cap();

  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      for (int j = 0; j < inner_loop_count; j++)
      {
        fiasco_pku_set(cap, 0, to_touch, utcb);
        *to_touch = j;
        fiasco_pku_set(cap, j % 16, to_touch, utcb);
      }
    }
    timer->Stop(i);
  }
}