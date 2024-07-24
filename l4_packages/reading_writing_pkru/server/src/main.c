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

extern unsigned int readPKRU(void);
extern void writePKRU(unsigned int pkruValue);
extern unsigned int testCPUID(void);

int
main(void)
{
  puts("Starting read Write Cycle of PKRU");
  unsigned int extendedFeaturesECXRegister = testCPUID();
  unsigned int ospkuMask =   0b00000000000000000000000000010000;
  unsigned int noWriteOnThese = 0b11;
  unsigned int currentWrite = 0;
  if(extendedFeaturesECXRegister & ospkuMask)
  {
    for (;;)
    {
      currentWrite++;
      unsigned int resultFirst = readPKRU();
      printf("Read PKRU %u\n", resultFirst);

      writePKRU(currentWrite & ~noWriteOnThese);

      printf("Wrote into pkru\n------------------------\n");
      
      sleep(1);
    }
  }
  else
  {
    printf("PKU is not enabled");
  }

}


