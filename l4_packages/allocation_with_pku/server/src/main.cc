/**
 * \file
 * \brief  Example of coarse grained memory allocation, in C++.
 */
/*
 * (c) 2009 Adam Lackorzynski <adam@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */


#include <l4/re/mem_alloc>
#include <l4/re/rm>
#include <l4/re/env>
#include <l4/re/dataspace>
#include <l4/re/util/cap_alloc>
#include <l4/sys/err.h>
#include <l4/sys/segment.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

extern "C" unsigned int readPKRU(void);
extern "C" void writePKRU(unsigned int pkruValue);

/**
 * \brief Allocate memory, given in bytes in the granularity of pages.
 *
 * \param size_in_bytes   Size to allocate, in bytes, will be truncates to
 *                          whole pages (L4_PAGESIZE).
 * \param flags           Flags to control memory allocation:
 *                          L4Re::Mem_alloc::Continuous:  Physically continuous memory
 *                          L4Re::Mem_alloc::Pinned:      Pinned memory
 *                          L4Re::Mem_alloc::Super_pages: Use big pages
 * \retval virt_addr      Virtual address the memory is accessible under,
 *                          undefined if return code != 0
 *
 * \return 0 on success, error code otherwise
 */
static int allocate_mem(unsigned long size_in_bytes, unsigned long flags,
                        void **virt_addr)
{
  int r;
  L4::Cap<L4Re::Dataspace> d;

  /* Allocate a free capability index for our data space */
  d = L4Re::Util::cap_alloc.alloc<L4Re::Dataspace>();
  if (!d.is_valid())
    return -L4_ENOMEM;

  size_in_bytes = l4_trunc_page(size_in_bytes);

  /* Allocate memory via a dataspace */
  if ((r = L4Re::Env::env()->mem_alloc()->alloc(size_in_bytes, d, flags)))
    return r;

  /* Make the dataspace visible in our address space */
  *virt_addr = 0;
  if ((r = L4Re::Env::env()->rm()->attach(virt_addr, size_in_bytes,
                                          L4Re::Rm::F::Search_addr | L4Re::Rm::F::RW,
                                          L4::Ipc::make_cap_rw(d), 0,
                                          flags & L4Re::Mem_alloc::Super_pages
                                            ? L4_SUPERPAGESHIFT : L4_PAGESHIFT)))
    return r;

  /* Done, virtual address is in virt_addr */
  return 0;
}

/**
 * \brief Free previously allocated memory.
 *
 * \param virt_addr    Virtual address return by allocate_mem
 *
 * \return 0 on success, error code otherwise
 */
static int free_mem(void *virt_addr)
{
  int r;
  L4::Cap<L4Re::Dataspace> ds;

  /* Detach memory from our address space */
  if ((r = L4Re::Env::env()->rm()->detach(virt_addr, &ds)))
    return r;

  /* Release and return capability slot to allocator */
  L4Re::Util::cap_alloc.free(ds, L4Re::Env::env()->task().cap());

  /* All went ok */
  return 0;
}

static int set_pku(void* addr, unsigned int key_to_associate)
{
  return fiasco_pku_set(L4Re::Env::env()->task().cap(), key_to_associate, addr, l4_utcb());
}

static int memoryAllocationLoop(unsigned int key_to_associate)
{
  void *virt;

  /* Allocate memory: 16k Bytes (usually) */
  if (allocate_mem(4 * L4_PAGESIZE, 0, &virt))
    return 1;

  memset(virt, 0x12, 4 * L4_PAGESIZE); // TODO LAZY PAGE ALLOCATION RUINS EVERYTHING.
  
  set_pku(virt, key_to_associate);
  set_pku(virt, key_to_associate);

  memset(virt, 0x14, 4 * L4_PAGESIZE);


  if (free_mem(virt))
    return 2;

  return 0;
}

int main(void)
{
  memoryAllocationLoop(0);
  unsigned int pkruvalue = 0b110011;
  writePKRU(~pkruvalue); //lock all keys except 0 and 2
  puts("disabled everything except key 0");
  sleep(1);


  memoryAllocationLoop(0);
  puts("key zero allocation done");
  sleep(1);


  memoryAllocationLoop(2);
  puts("key 2 allocation done");
  sleep(1);


  memoryAllocationLoop(1); // should crash
  puts("key 1 allocation done but should've crashed");
  sleep(1);

  memoryAllocationLoop(13);
  puts("key 13 allocation done but should've crashed");
  sleep(1);
}
