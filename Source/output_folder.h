/*
  ==============================================================================

    output_folder.h
    Created: 8 Jun 2020 7:37:19pm
    Author:  gvne

  ==============================================================================
*/

#pragma once

#include <map>
#include <string>
#include <JuceHeader.h>
#include "spleeter/spleeter.h"

class OutputFolder {
public:
  OutputFolder(const std::string &path);
  ~OutputFolder();

  /// Flush the remaining data
  void Flush();

  /// Write data and cross fade the last 1second with the end of the previous
  /// input
  void Write(const std::map<std::string, spleeter::Waveform> &data,
             std::error_code &err);
  
 private:
  std::string path_;
  std::map<std::string, std::shared_ptr<AudioFormatWriter>> writers_;
  std::map<std::string, spleeter::Waveform> previous_write_;
};
