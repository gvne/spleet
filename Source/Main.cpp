#include <JuceHeader.h>
#include <cxxopts.hpp>

#include "io.h"
#include "utils.h"

int main (int argc, char* argv[]) {
  cxxopts::Options options("spleet", "Split audio");
  options.add_options()
    ("i,input", "Input file path", cxxopts::value<std::string>())
    ("o,output", "Output directory path", cxxopts::value<std::string>()->default_value("./spleet"))
    ("c,count", "Number of output files", cxxopts::value<int>()->default_value("2"))
    ("h,help", "Print usage")
    ;
  
  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }
  
  // TODO: validate the parameters (output count) and initialize spleeter
  
  
  double input_sampling_rate = 0;
  const double process_sampling_rate = 44100;
  Eigen::MatrixXf data;
  std::error_code err;
  Read(result["input"].as<std::string>(), &data, &input_sampling_rate, err);
  if (err) {
    std::cout << "Couldn't read source file" << std::endl;
    return 0;
  }
  
  auto waveform = Resample(Stereo(data), input_sampling_rate, process_sampling_rate);
  // TODO: process
  
  Write(waveform, process_sampling_rate, result["output"].as<std::string>(), err);
  if (err) {
    std::cout << "Couldn't export at " << std::endl;
  }

  return 0;
}
