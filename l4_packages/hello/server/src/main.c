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
  for (int i = 0; i < 5; i++)
    {
      unsigned int extendedFeaturesECXRegister = testCPUID();
      int pkuMask =     0b00000000000000000000000000001000;
      int ospkuMask =   0b00000000000000000000000000010000;

      int pku_exists = (extendedFeaturesECXRegister & pkuMask) == 0b1000;
      int is_pku_enabled = (extendedFeaturesECXRegister & ospkuMask) == 0b10000;
      printf("pku feature exist:%d, is enabled: %d\n", pku_exists, is_pku_enabled);
      sleep(1);
    }

}


