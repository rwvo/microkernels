#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <map>
#include <hc.hpp>

//--- stuff for parsing of command line options
enum class opts { h2d, d2h, pinned, size, reps };

void parse_options(std::map<opts, std::size_t>& options, int argc, char** argv);


//---

// returns a vector of indices of "accelerators" that are actual GPU devices 
std::vector<int> get_gpu_indices(const std::vector<hc::accelerator>& accelerators);

#endif // _UTILS_H_
