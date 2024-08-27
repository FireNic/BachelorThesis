
// #include <../../../mpklibrary/include/pkrulib.h> // for development
// #include <../../../timingMPK/include/timer.h>
// #include <../../../timingMPK/include/protectablepage.h>
#include <l4/mpklibrary/pkrulib.h>
#include <l4/timingMPK/timer.h>
#include <l4/timingMPK/protectablepage.h>

#include <l4/re/env>
#include <l4/sys/segment.h>

#include <stdlib.h>
#include <stdio.h>

void SimpleSyscallLoop(MPKTimer *timer, int amount_of_results, int inner_loop_count, unsigned int *to_touch);

int main(void)
{
  printf("Simple Testing Syscall Loop says Hello\n");

  const int amount_of_results = 10000;
  const int inner_loop_count = 1000;

  PageAllocator PageAllocator;
  unsigned int *touching_this_memory = static_cast<unsigned int *>(PageAllocator.GetProtectablePage());
  PKRUlib::write(~0b11); // disable everything except key 0

  // Calling Setup of Timer
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  for (int i = 0; i < 10; i++)
  {
    SimpleSyscallLoop(&timer, amount_of_results, inner_loop_count, touching_this_memory);
  }
  // Testing
  SimpleSyscallLoop(&timer, amount_of_results, inner_loop_count, touching_this_memory);

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  printf("Results of Simple Testing Syscall Loop (count per time = %d) are:\n", inner_loop_count);
  puts(&results[0]);

  return 0;
}

void SimpleSyscallLoop(MPKTimer *timer, int amount_of_results, int inner_loop_count, unsigned int *to_touch)
{
  auto utcb = l4_utcb();
  auto cap = L4Re::Env::env()->task().cap();

  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      for (int j = 0; j < inner_loop_count; j++)
      {
        fiasco_pku_set(cap, j % 16, to_touch, utcb);
      }
    }
    timer->Stop(i);
  }
}