
//#include <pkrulib.h>
#include <l4/mpklibrary/pkrulib.h>
#include <stdio.h>


int main(void)
{
  printf("TimingTestSaysHello\n");
  auto key = PKRUlib::Key_5;
  auto rights = PKRUlib::AccessWriteDisable;
  PKRUlib::write(key, rights);
  bool success = PKRUlib::has_rights(key, rights);
  printf("WroteIntoPKRU with correct rights: %d\n", success);
  return 0;
}
