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
  unsigned int extendedFeaturesECXRegister = testCPUID();
  unsigned int ospkuMask =   0b00000000000000000000000000010000;
  unsigned int noWriteOnThese = 0b11;
  
  if(extendedFeaturesECXRegister & ospkuMask)
  {
    for (unsigned int currentWrite = 0; currentWrite < 40; currentWrite += 4)
    {
      
      unsigned int resultFirst = readPKRU();

      writePKRU(currentWrite & ~noWriteOnThese);

      printf("Read PKRU %u and Wrote newValue %u\n", resultFirst, currentWrite);
      
      sleep(1);
    }
  }
  else
  {
    printf("PKU is not enabled");
  }

}


