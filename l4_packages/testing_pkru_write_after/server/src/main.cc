
// #include <../../../mpklibrary/include/pkrulib.h> // for development
// #include <../../../timingMPK/include/timer.h>
#include <l4/mpklibrary/pkrulib.h>
#include <l4/timingMPK/timer.h>

#include <l4/re/env>
#include <l4/sys/segment.h>

#include <stdlib.h>
#include <stdio.h>


void PKRUWriteAndWriteAfter(MPKTimer *timer, int amount_of_results, unsigned int protected_with_key, unsigned int *protected_memory);

int main(void)
{
  printf("Testing PKRU Write After Loop says Hello\n");
  
  unsigned int protected_with_key = 1;
  unsigned int *touching_this_memory = static_cast<unsigned int*>(malloc(sizeof(*touching_this_memory)));
  fiasco_pku_set(L4Re::Env::env()->task().cap(), protected_with_key, touching_this_memory, l4_utcb());

  // Calling Setup of Timer
  const int amount_of_results = 1000;
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  PKRUWriteAndWriteAfter(&timer, amount_of_results, protected_with_key, touching_this_memory);
  // Testing
  PKRUWriteAndWriteAfter(&timer, amount_of_results, protected_with_key, touching_this_memory);  

  // Results
  std::vector<char> results = timer.ResultsForExport(',', ';');
  puts("Results of Testing PKRU Write After are:");
  puts(&results[0]);

  free(touching_this_memory);
  return 0;
}



void PKRUWriteAndWriteAfter(MPKTimer *timer, int amount_of_results, unsigned int protected_with_key, unsigned int *protected_memory)
{
  unsigned int disable_key = 0b11 << (protected_with_key * 2);
  PKRUlib::write(disable_key);

  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      PKRUlib::write(0);
      *protected_memory = i;
    }
    timer->Stop(i);
    PKRUlib::write(disable_key);
  }
  PKRUlib::write(0);
}