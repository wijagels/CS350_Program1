#ifndef _VPROC_H
#define _VPROC_H

#define TLB_MIN 12
#define TLB_MAX 4096
#define PAGE_MIN 5
#define PAGE_MAX 15

#define sandwich(a, b, c) ((a) <= (b) && (b) <= (c))

class VRuntime {
  public:
  VRuntime(unsigned tlb, unsigned page_size, double locality, unsigned life);

  private:
  unsigned tlb_size;
  unsigned page_size;
  double locality;
  unsigned avg_lifetime;
};


// VRuntime functions

#endif
