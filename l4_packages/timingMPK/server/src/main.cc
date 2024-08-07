
// #include <../../../mpklibrary/include/pkrulib.h> // for development
// #include <../../../timingMPK/include/timer.h>
#include <l4/mpklibrary/pkrulib.h>
#include <l4/timingMPK/timer.h>
#include <stdio.h>

void TestTemplate(MPKTimer *timer, int amount_of_results, PKRUlib::Key key, PKRUlib::Rights rights);

int main(void)
{
  printf("TestTemplateSaysHello\n");

  // Setup Memory
  const auto key = PKRUlib::Key_5;
  const auto rights = PKRUlib::AccessWriteDisable;

  // Calling Setup of Timer
  const int amount_of_results = 5;
  MPKTimer timer = MPKTimer(amount_of_results);

  // Warmup
  TestTemplate(&timer, amount_of_results, key, rights);
  // Testing
  TestTemplate(&timer, amount_of_results, key, rights);  

  // Results
  auto results = timer.ResultsForExport(',', '\n');
  puts("RESULTS OF TEST ARE:");
  puts(&results[0]);

  return 0;
}

void TestTemplate(MPKTimer *timer, int amount_of_results, PKRUlib::Key key, PKRUlib::Rights rights)
{
  // Outside Loop for averaging Results
  for (int i = 0; i < amount_of_results; i++)
  {
    timer->Start();
    {
      // Single Testing (Either One Instruction or a Loop)
      PKRUlib::write(key, rights);
      // End Of Single Test
    }
    timer->Stop(i);
  }
}
