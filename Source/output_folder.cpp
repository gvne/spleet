/*
  ==============================================================================

    output_folder.cpp
    Created: 8 Jun 2020 7:37:19pm
    Author:  gvne

  ==============================================================================
*/

#include "output_folder.h"
#include <algorithm>
#include "constant.h"

namespace {

std::string JoinPath(const std::string parent, const std::string &child) {
  return parent + "/" + child;
}

template <typename SignalType>
void Write(SignalType signal, std::shared_ptr<AudioFormatWriter> writer) {
  std::vector<float *> array_data;
  std::vector<std::vector<float>> vec_data;
  for (auto channel_idx = 0; channel_idx < signal.rows(); channel_idx++) {
    std::vector<float> channel_data(signal.cols());
    Eigen::Map<Eigen::VectorXf>(channel_data.data(), signal.cols()) =
        signal.row(channel_idx);
    vec_data.emplace_back(std::move(channel_data));
    array_data.push_back(vec_data[channel_idx].data());
  }
  writer->writeFromFloatArrays(array_data.data(), signal.rows(), signal.cols());
}

} // namespace

OutputFolder::OutputFolder(const std::string &path) : path_(path) {}

OutputFolder::~OutputFolder() { Flush(); }

void OutputFolder::Flush() {
  // write the remaining data
  for (auto previous_write : previous_write_) {
    ::Write(previous_write.second, writers_[previous_write.first]);
  }

  for (auto writer : writers_) {
    writer.second->flush();
  }
}

void OutputFolder::Write(const std::map<std::string, spleeter::Waveform> &data,
                         std::error_code &err) {
  for (auto waveform : data) {
    auto channel_count = waveform.second.rows();
    auto frame_count = waveform.second.cols();

    // If no writer found, create it
    if (writers_.find(waveform.first) == std::end(writers_)) {
      File output_file(::JoinPath(path_, waveform.first + ".wav"));
      if (output_file.existsAsFile()) { // If file already exists, delete it
        output_file.deleteFile();
      }
      WavAudioFormat format;
      auto writer = std::shared_ptr<AudioFormatWriter>(format.createWriterFor(
          output_file.createOutputStream(), kProcessSamplingRate, channel_count,
          16, StringPairArray(), 0));
      writers_[waveform.first] = writer;
      previous_write_[waveform.first] = spleeter::Waveform();
    }

    // Cross fade the beginning with the kept buffer
    auto previous_write_frame_count = previous_write_[waveform.first].cols();
    Eigen::VectorXf fade_in(previous_write_frame_count);
    Eigen::VectorXf fade_out(previous_write_frame_count);
    for (auto idx = 0; idx < previous_write_frame_count; idx++) {
      fade_in(idx) = static_cast<float>(idx) / previous_write_frame_count;
      fade_out(idx) = 1.0 - fade_in(idx);
    }
    for (auto row = 0; row < previous_write_[waveform.first].rows(); row++) {
      auto data =
          waveform.second.row(row).segment(0, previous_write_frame_count);
      auto previous_data = previous_write_[waveform.first].row(row);
      data.array() = (data.array() * fade_in.transpose().array()) +
                     (previous_data.array() * fade_out.transpose().array());
    }

    // Keep the last second
    auto frame_to_keep =
        std::min(static_cast<int>(kProcessSamplingRate * kBatchOverlapSeconds),
                 static_cast<int>(waveform.second.cols()));
    previous_write_[waveform.first] = waveform.second.rightCols(frame_to_keep);

    // Write to disk
    auto frame_to_write = frame_count - frame_to_keep;
    ::Write(waveform.second.leftCols(frame_to_write), writers_[waveform.first]);
  }
}
