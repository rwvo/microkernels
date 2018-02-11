#include <hc.hpp>
#include <vector>
#include <iostream>
#include <map>

#include "utils.h"

std::map<opts, std::size_t> init_options(){
  std::map<opts, std::size_t> options;
  
  options[opts::h2d] = 0;
  options[opts::d2h] = 0;
  options[opts::pinned] = 0;
  options[opts::gpu] = 1; // bitmask denoting gpu devices used, e.g. 11 = 1 + 2 + 8: use devices 1, 2, and 4.
  options[opts::size] = 0;
  options[opts::reps] = 1;

  return options;
}

int main(int argc, char** argv){
  auto options = init_options();
  parse_options(options, argc, argv);
  
  auto accelerators = hc::accelerator::get_all();
  std::wcerr << "done\n";
}
