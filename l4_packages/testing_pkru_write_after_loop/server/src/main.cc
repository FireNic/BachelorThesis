
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

void PKRUWriteAndWriteAfter(MPKTimer *timer, int amount_of_results, int inner_loop_count, unsigned int protected_with_key, unsigned int *protected_memory);

int main(void)
{
  printf("Testing PKRU Write After Loop says Hello\n");

  unsigned int protected_with_key = 1;
  PageAllocator PageAllocator;
  unsigned int *touching_this_memory = static_cast<unsigned int *>(PageAllocator.GetProtectablePage());
  fiasco_pku_set(L4Re::Env::env()->task().cap(), protected_with_key, touching_this_memory, l4_utcb());

  // Calling Setup of Timer
  const int amount_of_results = 10000;
  const int amount_of_inner_loops = 10000;
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  for (int i = 0; i < 10; i++)
  {
    PKRUWriteAndWriteAfter(&timer, amount_of_results, amount_of_inner_loops, protected_with_key, touching_this_memory);
  }
  // Testing
  PKRUWriteAndWriteAfter(&timer, amount_of_results, amount_of_inner_loops, protected_with_key, touching_this_memory);

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  printf("Results of Testing PKRU Write After Loop (count per time = %d) are:\n", amount_of_inner_loops);
  puts(&results[0]);

  free(touching_this_memory);
  return 0;
}

void PKRUWriteAndWriteAfter(MPKTimer *timer, int amount_of_results, int inner_loop_count, unsigned int protected_with_key, unsigned int *protected_memory)
{
  unsigned int disable_key = 0b11 << (protected_with_key * 2);
  PKRUlib::write(disable_key);
  // Outside Loop for averaging Results
  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      // Single Testing (Either One Instruction or a Loop)
      for (int j = 0; j < inner_loop_count; j++)
      {
        PKRUlib::write(0);
        *protected_memory = j * 2513 / (j + 523);
        PKRUlib::write(disable_key);
      }
      // End Of Single Test
    }
    timer->Stop(i);
  }
  PKRUlib::write(0);
}