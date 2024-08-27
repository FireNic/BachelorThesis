

// #include <../../../timingMPK/include/timer.h>
#include <l4/timingMPK/timer.h>
#include <stdio.h>

void TestTemplate(MPKTimer *timer, int amount_of_results);

int main(void)
{
  printf("TestTemplateSaysHello\n");

  // Calling Setup of Timer
  const int amount_of_results = 10000;
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  for (int i = 0; i < 1000; i++)
  {
    TestTemplate(&timer, amount_of_results);
  }
  // Testing
  TestTemplate(&timer, amount_of_results);

  // Results
  auto results = timer.ResultsForExport(',', ';');
  puts("RESULTS OF TEST ARE:");
  puts(&results[0]);

  return 0;
}

void TestTemplate(MPKTimer *timer, int amount_of_results)
{
  // Outside Loop for averaging Results
  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      // Single Testing (Either One Instruction or a Loop)
      // End Of Single Test
    }
    timer->Stop(i);
  }
}
