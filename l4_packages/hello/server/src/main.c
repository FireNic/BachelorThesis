/*
 * (c) 2008-2009 Adam Lackorzynski <adam@os.inf.tu-dresden.de>,
 *               Frank Mehnert <fm3@os.inf.tu-dresden.de>,
 *               Lukas Grützmacher <lg2@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern unsigned int testCPUID(void);

int
main(void)
{
  for (;;)
    {
      unsigned int extendedFeaturesECXRegister = testCPUID();
      {
        puts("------------------");
        printf("testCPUID finished with %u\n", extendedFeaturesECXRegister);
      }
      int pkuMask =     0b00000000000000000000000000001000;
      int ospkuMask =   0b00000000000000000000000000010000;

      if((extendedFeaturesECXRegister & pkuMask) == 0b1000){
          puts("pku feature exists");
      }
      else{
          puts("pku feature does not exist");
      }

      if((extendedFeaturesECXRegister & ospkuMask) == 0b10000){
          puts("pku feature is enabled");
      }
      else{
          puts("pku feature is not enabled");
      }

      sleep(1);
    }

}


