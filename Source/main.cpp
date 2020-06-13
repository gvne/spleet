#include <JuceHeader.h>
#include <cxxopts.hpp>
#include "spleeter/spleeter.h"

//#include "io.h"
#include "input_file.h"
#include "output_folder.h"
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
    std::cerr << "Invalid stem count. Only 2/4/5 supported." << std::endl;
    return -1;
  }

  // validate output directory
  auto output = result["output"].as<std::string>();
  File output_file(output);
  if (!(output_file.exists() && output_file.isDirectory())) {
    std::cerr << "Output folder " << output << " does not seem to exist" << std::endl;
    return -1;
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
    std::cerr << "Couldn't initialize spleeter" << std::endl;
    return -1;
  }

  InputFile input(result["input"].as<std::string>());
  input.Open(err);
  if (err) {
    std::cerr << "Couldn't read source file" << std::endl;
    return -1;
  }
  
  OutputFolder output_folder(output);
  
  while (true) {
    auto data = input.Read();
    if (data.cols() == 0) {
      return 0;
    }
    auto result = Split(data, separation_type, err);
    if (err) {
      std::cerr << "Failed to split" << std::endl;
      return -1;
    }
    output_folder.Write(result, err);
    if (err) {
      std::cerr << "Failed to export" << std::endl;
      return -1;
    }
  }
  return 0;
}
