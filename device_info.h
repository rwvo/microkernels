#ifndef _DEVICE_INFO_H_
#define _DEVICE_INFO_H_

#include <hc.hpp>

class device_info {
public:
  device_info();
  void show() const;
private:
  const std::vector<hc::accelerator> m_accelerators;
  const std::vector<int> m_gpu_indices;
};

#endif // _DEVICE_INFO_
