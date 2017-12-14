#include "utils.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>


void exit_with_usage(std::map<opts, std::size_t>& options, char** argv){
  // todo: list/document the enabled options
  std::wcerr << argv[0] << ": invalid option\n";
  exit(1);
}


void parse_options(std::map<opts, std::size_t>& options, int argc, char** argv){
// options: map of opts to integers for options enabled by the calling benchmark
  std::map<std::string, opts> str2opt;
  str2opt["h2d"] = opts::h2d;
  str2opt["d2h"] = opts::d2h;
  str2opt["pinned"] = opts::pinned;
  str2opt["-s"] = opts::size;
  str2opt["-r"] = opts::reps;
  
  for(int arg_no = 1; arg_no < argc; ++ arg_no){
    // first check whether the option is known at all, i.e., if it is a string that
    // maps to an option in any of the benchmarks
    std::string arg_str(argv[arg_no]);
    auto str2opt_iter = str2opt.find(arg_str);
    if(str2opt_iter == str2opt.end()){
      exit_with_usage(options, argv);
    }
    // next, check whether the option is enables for the benchmark from which
    // we call parse_options()
    auto opt = str2opt_iter->second; // str2opt_iter points to {string, opts} pair
    auto opts_iter = options.find(opt);
    if(opts_iter == options.end()){
      exit_with_usage(options, argv);
    }
    
    if(arg_str[0] == '-'){ // parse next argument, which must be numeric and positive
      // number may optionally be followed by 'm' (mega) or 'g' (giga) suffix
      if(++arg_no >= argc){
	exit_with_usage(options, argv);
      }
      std::string num_str(argv[arg_no]);
      if(num_str[0] == '-'){
	exit_with_usage(options, argv);
      }
      std::istringstream iss(num_str);
      std::size_t value{0}; // required positive number
      char suffix{0}; // optional 'm', 'M' or 'g', 'G'
      char tail{0};   // should be empty
      iss >> value >> suffix >> tail;
      if(value == 0 or tail != 0){
	exit_with_usage(options, argv);
      }
      switch(suffix){
      case 0:
	break;
      case 'm': case 'M':
	value *= 1 << 20;
	break;
      case 'g': case 'G':
	value *= 1 << 30;
	break;
      default:
	exit_with_usage(options, argv);
	break;
      }
      options[opt] = value;
    } else { // boolean option; turn it on
      options[opt] = 1; 
    }
  }
}


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
