/*
  ==============================================================================

    split.cpp
    Created: 1 Jun 2020 3:14:04pm
    Author:  gvne

  ==============================================================================
*/

#include "split.h"
#include "io.h"

std::string JoinPath(const std::string parent, const std::string &child) {
  return parent + "/" + child;
}

void SplitTwo(const spleeter::Waveform &waveform,
              const std::string &destination, double sampling_rate,
              std::error_code &err) {
  spleeter::Waveform vocals, accompaniment;
  spleeter::Split(waveform, &vocals, &accompaniment, err);
  if (err) {
    return;
  }
  Write(vocals, sampling_rate, JoinPath(destination, "vocals.wav"), err);
  Write(accompaniment, sampling_rate,
        JoinPath(destination, "accompaniment.wav"), err);
}

void SplitFour(const spleeter::Waveform &waveform,
               const std::string &destination, double sampling_rate,
               std::error_code &err) {
  spleeter::Waveform vocals, drums, bass, other;
  spleeter::Split(waveform, &vocals, &drums, &bass, &other, err);
  if (err) {
    return;
  }
  Write(vocals, sampling_rate, JoinPath(destination, "vocals.wav"), err);
  Write(drums, sampling_rate, JoinPath(destination, "drums.wav"), err);
  Write(bass, sampling_rate, JoinPath(destination, "bass.wav"), err);
  Write(other, sampling_rate, JoinPath(destination, "other.wav"), err);
}

void SplitFive(const spleeter::Waveform &waveform,
               const std::string &destination, double sampling_rate,
               std::error_code &err) {
  spleeter::Waveform vocals, drums, bass, piano, other;
  spleeter::Split(waveform, &vocals, &drums, &bass, &piano, &other, err);
  if (err) {
    return;
  }
  Write(vocals, sampling_rate, JoinPath(destination, "/vocals.wav"), err);
  Write(drums, sampling_rate, JoinPath(destination, "/drums.wav"), err);
  Write(bass, sampling_rate, JoinPath(destination, "/bass.wav"), err);
  Write(bass, sampling_rate, JoinPath(destination, "/piano.wav"), err);
  Write(other, sampling_rate, JoinPath(destination, "/other.wav"), err);
}

void Split(const spleeter::Waveform &waveform, spleeter::SeparationType type,
           const std::string &destination, double sampling_rate,
           std::error_code &err) {
  switch (type) {
  case spleeter::TwoStems:
    SplitTwo(waveform, destination, sampling_rate, err);
    break;
  case spleeter::FourStems:
    SplitFour(waveform, destination, sampling_rate, err);
    break;
  case spleeter::FiveStems:
    SplitFive(waveform, destination, sampling_rate, err);
    break;
  default:
    err = std::make_error_code(std::errc::not_supported);
    break;
  }
}
