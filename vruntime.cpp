#include "vruntime.hpp"
#include <cassert>

// VRuntime impl

/**
 *  \brief Describes the runtime of a virtual machine
 *
 *  This struct contains information that will be used to simulate
 *  the runtime of several processes on our virtual machine for memory
 *  profiling.
 *
 *  \param tlb Size of the TLB will be tlb
 *  \param page_size Size of each page will be 2^(page_size)
 *  \param locality The probability that the next memory reference will be near the last.
 *  0 <= locality <= 1
 *  \param avg_lifetime The average amount of time a process will be alive
 *  \return return type
 */
VRuntime::VRuntime(unsigned tlb, unsigned page_size, double locality, unsigned life):
  tlb_size{tlb}, page_size{page_size}, locality{locality}, avg_lifetime{life} {

  assert(sandwich(TLB_MIN, tlb, TLB_MAX) && sandwich(PAGE_MIN, page_size, PAGE_MAX));
}
