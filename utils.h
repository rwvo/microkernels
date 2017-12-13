#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>

// returns a vector of indices of "accelerators" that are actual GPU devices 
std::vector<int> get_gpu_indices(const std::vector<hc::accelerator>& accelerators){
  std::vector<int> gpu_indices;
  for(std::size_t i=0; i!= accelerators.size(); ++i){
    if(accelerators[i].get_device_path() != L"cpu"){
      gpu_indices.push_back(i);
    }
  }
  return gpu_indices;
}

#endif // _UTILS_H_
