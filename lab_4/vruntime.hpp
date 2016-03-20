#ifndef _VPROC_H
#define _VPROC_H

#include <iostream>
#include <vector>

#define PAGE_MIN 5
#define PAGE_MAX 15
#define ADDR_MIN 8
#define ADDR_MAX 64

#define sandwich(a, b, c) ((a) <= (b) && (b) <= (c))

class Process {
  public:
  static const double LIFE_VARIANCE;
  static const unsigned LIFE_MIN;
  Process(): _id{_next_id++}, _age{0}, _death_time{0}, _last_addr{0} {}
  Process(const Process&) = default;
  Process& operator=(const Process&) = default;
  explicit Process(unsigned death): _id{_next_id++}, _age{0}, _death_time{death}, _last_addr{0} {}
  ~Process() = default;
  inline bool is_alive() { return _age < _death_time; }
  void access(unsigned long long);
  unsigned last_addr() { return _last_addr; }
  inline unsigned id() { return _id; }

  private:
  unsigned _age;
  unsigned _death_time;
  unsigned long long _last_addr;
  unsigned _id;
  static unsigned _next_id;
};

class VRuntime {
  public:
  VRuntime() = delete;
  VRuntime(unsigned procs, unsigned addr_size, unsigned page_size, double locality, unsigned life);
  ~VRuntime() = default;
  VRuntime(const VRuntime&) = delete;
  VRuntime& operator=(const VRuntime&) = delete;
  friend std::ostream& operator<<(std::ostream&, const VRuntime&);

  private:
  unsigned addr_size;
  unsigned page_size;
  double locality;
  unsigned avg_lifetime;
  unsigned max_procs;
};


// VRuntime functions

#endif
