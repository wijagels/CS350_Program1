#ifndef _VPROC_H
#define _VPROC_H

#include <iostream>
#include <vector>

#define TLB_MIN 12
#define TLB_MAX 4096
#define PAGE_MIN 5
#define PAGE_MAX 15

#define sandwich(a, b, c) ((a) <= (b) && (b) <= (c))

class Process {
  public:
  static const double LIFE_VARIANCE;
  static const unsigned LIFE_MIN;
  Process() = default;
  Process(unsigned death): _age{0}, _death_time{death}, _last_addr{0} {}
  ~Process() = default;
  inline bool is_alive() { return _age < _death_time; }
  void access(unsigned);
  private:
  unsigned _age;
  unsigned _death_time;
  unsigned _last_addr;
};

// TODO Parameterize the variance
const double Process::LIFE_VARIANCE = 40.0;
const unsigned Process::LIFE_MIN = 10u;

class VRuntime {
  public:
  VRuntime(unsigned procs, unsigned tlb, unsigned page_size, double locality, unsigned life);
  ~VRuntime() = default;
  VRuntime(const VRuntime&) = delete;
  VRuntime& operator=(const VRuntime&) = delete;
  friend std::ostream& operator<<(std::ostream&, const VRuntime&);

  private:
  unsigned tlb_size;
  unsigned page_size;
  double locality;
  unsigned avg_lifetime;
  unsigned max_procs;
};


// VRuntime functions

#endif
