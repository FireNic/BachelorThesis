
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

void SimpleSyscallWrite(MPKTimer *timer, int amount_of_results, unsigned int *to_touch);

int main(void)
{
  printf("Simple Testing Syscall Write says Hello\n");

  const int amount_of_results = 10000;
  PageAllocator PageAllocator;
  unsigned int *touching_this_memory = static_cast<unsigned int *>(PageAllocator.GetProtectablePage());
  PKRUlib::write(~0b11); // disable everything except key 0

  // Calling Setup of Timer
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  for (int i = 0; i < 1000; i++)
  {
    SimpleSyscallWrite(&timer, amount_of_results, touching_this_memory);
  }
  // Testing
  SimpleSyscallWrite(&timer, amount_of_results, touching_this_memory);

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  puts("Results of Simple Testing Syscall Write are:");
  puts(&results[0]);

  return 0;
}

void SimpleSyscallWrite(MPKTimer *timer, int amount_of_results, unsigned int *to_touch)
{
  auto utcb = l4_utcb();
  auto cap = L4Re::Env::env()->task().cap();

  for (int i = 0; i < amount_of_results; i++)
  {
    fiasco_pku_set(cap, i % 16, to_touch, utcb);
    timer->Start();
    {
      fiasco_pku_set(cap, 0, to_touch, utcb);
      *to_touch = i;
    }
    timer->Stop(i);
  }
}