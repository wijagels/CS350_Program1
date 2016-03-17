#ifndef _VPROC_H
#define _VPROC_H

#define TLB_MIN 12
#define TLB_MAX 4096
#define PAGE_MIN 5
#define PAGE_MAX 15

#define sandwich(a, b, c) ((a) <= (b) && (b) <= (c))

typedef struct {
  unsigned tlb_size;
  unsigned page_size;
  double locality;
  unsigned avg_lifetime;
} VRuntime;

// VRuntime functions
VRuntime *VRuntime_new(unsigned, unsigned, double, unsigned);
void VRuntime_del(VRuntime *);

#endif
