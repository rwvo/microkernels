#ifndef _DEVICE_INFO_H_
#define _DEVICE_INFO_H_

#include <hc.hpp>

class device_info {
public:
  device_info();
  void show() const;
  bool is_gpu_index(size_t i) const;
  bool validate_gpu_indices(const std::vector<size_t>& indices) const;
private:
  const std::vector<hc::accelerator> m_accelerators;
  const std::vector<size_t> m_gpu_indices;
};

#endif // _DEVICE_INFO_
