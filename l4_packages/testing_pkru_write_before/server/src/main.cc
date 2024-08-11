
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

void PKRUWriteAndWriteBefore(MPKTimer *timer, int amount_of_results, unsigned int protected_with_key, unsigned int *protected_memory);

int main(void)
{
  printf("Testing PKRU Write Before says Hello\n");
  
  unsigned int protected_with_key = 1;
  PageAllocator PageAllocator;
  unsigned int *touching_this_memory = static_cast<unsigned int *>(PageAllocator.GetProtectablePage());
  fiasco_pku_set(L4Re::Env::env()->task().cap(), protected_with_key, touching_this_memory, l4_utcb());

  // Calling Setup of Timer
  const int amount_of_results = 1000;
  MPKTimer timer = MPKTimer(amount_of_results);
  puts("after before first");
  // Warmup
  PKRUWriteAndWriteBefore(&timer, amount_of_results, protected_with_key, touching_this_memory);
  puts("after first");
  // Testing
  PKRUWriteAndWriteBefore(&timer, amount_of_results, protected_with_key, touching_this_memory);  

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  puts("Results of Testing PKRU Write Before are:");
  puts(&results[0]);

  return 0;
}



void PKRUWriteAndWriteBefore(MPKTimer *timer, int amount_of_results, unsigned int protected_with_key, unsigned int *protected_memory)
{
  unsigned int disable_key = 0b11 << (protected_with_key * 2);

  for (int i = 0; i < amount_of_results; i++)
  {
    PKRUlib::write(0);
    timer->Start();
    { 
      *protected_memory = i;
      PKRUlib::write(disable_key);
    }
    timer->Stop(i);
  }
  PKRUlib::write(0);
}