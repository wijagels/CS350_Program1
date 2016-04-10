#ifndef _VPROC_H
#define _VPROC_H

#include <iostream>
#include <vector>
#include <memory>

#define PAGE_MIN 5
#define PAGE_MAX 15
#define ADDR_MIN 8
#define ADDR_MAX 64

#define sandwich(a, b, c) ((a) <= (b) && (b) <= (c))

class Process {
  public:
  static const double LIFE_VARIANCE;
  static const int LIFE_MIN;
	using Policy = enum { NORMAL, OPEN, HEAVY, IO };
  Process():
		_id{++_next_id}, _age{0}, _death_time{0}, _addr_size{0}, _page_size{0} {}
  Process(const Process&) = default;
	Process(Process&&) = default;
  Process& operator=(const Process&) = default;
	Process& operator=(Process&&) = default;
  Process(unsigned addr_sz, unsigned pg_sz, int death, double locality = 0.5):
		_id{++_next_id}, _age{0}, _death_time{death}, _addr_size{addr_sz}, _page_size{pg_sz},
		_locality{locality},
		_refs{std::unique_ptr<std::vector<unsigned long long> >(new std::vector<unsigned long long>)}
	{}
  ~Process() = default;
  inline bool is_alive() { return _age < _death_time; }
  bool access(unsigned long long);
  unsigned last_addr() { return _refs->back(); }
  inline unsigned id() { return _id; }
  // Method will return a stream of addresses for a phase
	int run();
	const std::vector<unsigned long long> history() { return *_refs; }

  private:
  unsigned _id;
  int _age;
  int _death_time;
	unsigned _addr_size;
	unsigned _page_size;
	Policy _policy;
	std::unique_ptr<std::vector<unsigned long long> > _refs;
	double _locality;
  static unsigned _next_id;
};

class VRuntime {
  public:
  VRuntime() = delete;
  VRuntime(unsigned procs, unsigned addr_size, unsigned page_size, double locality, int life);
  ~VRuntime() = default;
  VRuntime(const VRuntime&) = delete;
  VRuntime& operator=(const VRuntime&) = delete;
  friend std::ostream& operator<<(std::ostream&, const VRuntime&);

  private:
  double locality;
  int avg_lifetime;
  unsigned max_procs;
};


// VRuntime functions

#endif
