#include <hc.hpp>
#include <vector>
#include <iostream>
#include "utils.h"


int main(){
  auto accelerators = hc::accelerator::get_all();
  std::wcerr << "done\n";
}
