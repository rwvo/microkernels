#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <map>
#include <hc.hpp>

//--- stuff for parsing of command line options
enum class opts {
    h2d,    // host to device copy
    d2h,    // device to host copy
    pinned, // host memory pinned/unpinned
    gpu,    // gpu devices involved in copy
    size,   // size of data transfers
    reps    // number of times copy is repeated
};

void parse_options(std::map<opts, std::size_t>& options, int argc, char** argv);

//--- 
std::vector<size_t> extract_gpu_indices(size_t gpu_mask);

#endif // _UTILS_H_
