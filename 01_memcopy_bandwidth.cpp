#include <hc.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <map>

#include "device_info.h"
#include "copy_operation.h"
#include "scoped_timers.h"
#include "utils.h"

std::map<opts, std::size_t> init_options(){
  std::map<opts, std::size_t> options;
  
  options[opts::h2d] = 0;
  options[opts::d2h] = 0;
  options[opts::pinned] = 0;
  options[opts::gpu] = 1; // bitmask denoting gpu devices used,
                          // e.g. 11 = 1 + 2 + 8: use devices 1, 2, and 4.
  options[opts::size] = 0;
  options[opts::reps] = 1;

  return options;
}

int main(int argc, char** argv){
  device_info dev_info;
  auto options = init_options();
  parse_options(options, argc, argv);

  auto gpus_used = extract_gpu_indices(options[opts::gpu]);
  for(auto i: gpus_used){
    std::wcerr << "using GPU " << i << '\n';
  }

  if (!dev_info.validate_gpu_indices(gpus_used)){ exit(1); };

  using operation = copy_operation<int>;
  std::vector<std::shared_ptr<operation>> operations;

  float tm;
  {
    SystemTimer timer(tm);
    if(options[opts::h2d]){
      for(auto gpu: gpus_used){
	operations.push_back(std::make_shared<operation>(0, gpu, options[opts::size],
							 options[opts::pinned],
							 dev_info));
      }
    }
    if(options[opts::d2h]){
      for(auto gpu: gpus_used){
	operations.push_back(std::make_shared<operation>(gpu, 0, options[opts::size],
							 options[opts::pinned],
							 dev_info));
      }
    }
  }

  auto Gib = operations.size() * ( static_cast<float>(options[opts::size]) / (1 << 30) );
  std::wcerr << "Allocated " << Gib << " Gib on the host, and "
	     <<  operations.size() << " * " << Gib/operations.size() << " Gib on "
	     << gpus_used.size() << " GPUs in " << tm << " seconds\n";
  
  
  for(auto op: operations){
    op->print_info();
  }
  
  {
    std::wcerr << "starting copies\n";
    SystemTimer timer(tm);
    for(auto op: operations){
      op->copy_async();
    }
    
    for(auto op: operations){
      op->wait();
    }
  }

  auto bw = Gib / tm;

  std::wcerr << "Copied " << Gib << " Gib in " << tm << " seconds (" << bw << " Gib/s)\n";
  
  std::wcerr << "done\n";
}
