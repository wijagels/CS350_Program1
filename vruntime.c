#include "vruntime.h"
#include "debug.h"

#include <stdbool.h>
#include <stdlib.h>

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
VRuntime *VRuntime_new(unsigned tlb, unsigned page_size, double locality, unsigned life) {

  // Bounds checking
  if (!sandwich(TLB_MIN, tlb, TLB_MAX)) {
    loge("TLB size invalid, %u <= %u <= %u", TLB_MIN, tlb, TLB_MAX);
    return NULL;
  } else if (!sandwich(PAGE_MIN, page_size, PAGE_MAX)) {
    loge("Page size invalid, 2^%u <= 2^%u <= 2^%u", PAGE_MIN, page_size, PAGE_MAX);
    return NULL;
  }

  VRuntime *vruntime = malloc(sizeof(VRuntime));
  vruntime->tlb_size = tlb;
  vruntime->page_size = page_size;
  vruntime->locality = locality;
  vruntime->avg_lifetime = life;

  return vruntime;
}

void VRuntime_del(VRuntime *vruntime) {
  // Close any open files
  free(vruntime);
  vruntime = NULL;
}
