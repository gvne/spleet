#include <JuceHeader.h>
#include <cxxopts.hpp>
#include "spleeter/spleeter.h"

#include "io.h"
#include "utils.h"
#include "split.h"

int main(int argc, char *argv[]) {
  auto working_directory = File::getCurrentWorkingDirectory().getFullPathName().toStdString();
  
  cxxopts::Options options("spleet", "Split audio");
  options.add_options()
    ("i,input", "Input file path", cxxopts::value<std::string>())
    ("o,output", "Output directory path",
     cxxopts::value<std::string>()->default_value(working_directory))
    ("c,count", "Number of output files",
     cxxopts::value<int>()->default_value("2"))
    ("h,help", "Print usage");

  auto result = options.parse(argc, argv);
  if (result.count("help") || !result.count("input")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  // validate the parameters (output count)
  std::error_code err;
  spleeter::SeparationType separation_type;
  switch (result["count"].as<int>()) {
  case 2:
    separation_type = spleeter::TwoStems;
    break;
  case 4:
    separation_type = spleeter::FourStems;
    break;
  case 5:
    separation_type = spleeter::FiveStems;
    break;
  default:
    std::cout << "Invalid stem count. Only 2/4/5 supported." << std::endl;
    return 0;
  }

  // validate output directory
  auto output = result["output"].as<std::string>();
  File output_file(output);
  if (!(output_file.exists() && output_file.isDirectory())) {
    std::cout << "Output folder " << output << " does not seem to exist" << std::endl;
    return 0;
  }

  // Initialize spleeter
  auto model_path =
      File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile)
          .getParentDirectory()
          .getChildFile("models")
          .getFullPathName()
          .toStdString();
  spleeter::Initialize(model_path, {separation_type}, err);
  if (err) {
    std::cout << "Couldn't initialize spleeter" << std::endl;
    return 0;
  }

  // Read input file
  double input_sampling_rate = 0;
  const double process_sampling_rate = 44100;
  Eigen::MatrixXf data;
  Read(result["input"].as<std::string>(), &data, &input_sampling_rate, err);
  if (err) {
    std::cout << "Couldn't read source file" << std::endl;
    return 0;
  }

  // resample it and convert it to stereo
  spleeter::Waveform waveform =
      Resample(Stereo(data), input_sampling_rate, process_sampling_rate);

  // process and export
  Split(waveform, separation_type, output, process_sampling_rate, err);
  if (err) {
    std::cout << "Export failed" << std::endl;
    return 0;
  }

  return 0;
}
