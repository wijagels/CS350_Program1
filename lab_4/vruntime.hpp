#ifndef _VPROC_H
#define _VPROC_H

#include <iostream>
#include <vector>
#include <memory>

#define PAGE_MIN 5u
#define PAGE_MAX 15u
#define ADDR_MIN 8u
#define ADDR_MAX 63u

#define sandwich(a, b, c) ((a) <= (b) && (b) <= (c))

namespace {
	// How far forward and backward some is "local"
	const unsigned LOCAL_REACH = 5;
}

class Process {
  public:
  static const double LIFE_STDEV;
  static const int LIFE_MIN;
	static const unsigned ADDRESS_MIN;
	using Policy = enum { NORMAL, OPEN, HEAVY, IO };
  Process() = delete;
  Process(const Process&) = default;
	Process(Process&&) = default;
  Process& operator=(const Process&) = default;
	Process& operator=(Process&&) = default;
  Process(unsigned addr_sz, unsigned pg_sz, int death, double locality, unsigned phase):
		_id{++_next_id}, _age{0}, _death_time{death}, _addr_size{addr_sz}, _page_size{pg_sz},
		_locality{locality}, _phase_length{phase},
		_refs{std::unique_ptr<std::vector<unsigned long long> >(new std::vector<unsigned long long>{0})}
	{}
  ~Process() = default;
  inline bool is_alive() { return _age < _death_time; }
  bool access(unsigned long long);
  unsigned last_addr() { return _refs->back(); }
  inline unsigned id() { return _id; }
  // Method will return a stream of addresses for a phase
	int run();
	const std::vector<unsigned long long>& history() { return *_refs; }

  private:
  unsigned _id;
  int _age;
  int _death_time;
	unsigned _addr_size;
	unsigned _page_size;
	Policy _policy;
	double _locality;
	unsigned _phase_length;
	std::unique_ptr<std::vector<unsigned long long> > _refs;
  static unsigned _next_id;
};

class VRuntime {
  public:
  VRuntime() = delete;
  VRuntime(unsigned procs, unsigned addr_size, unsigned page_size,
			double locality, int life, double v, unsigned phase);
  ~VRuntime() = default;
  VRuntime(const VRuntime&) = delete;
  VRuntime& operator=(const VRuntime&) = delete;
  friend std::ostream& operator<<(std::ostream&, const VRuntime&);

  private:
	unsigned max_procs, addr_size, page_size;
  double locality;
  int avg_lifetime;
	double address_stdev;
	unsigned phase_length;
};


// VRuntime functions

#endif
