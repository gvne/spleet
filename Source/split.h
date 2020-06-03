/*
  ==============================================================================

    split.h
    Created: 1 Jun 2020 3:13:58pm
    Author:  gvne

  ==============================================================================
*/

#pragma once

#include "spleeter/spleeter.h"

void Split(const spleeter::Waveform &waveform, spleeter::SeparationType type,
           const std::string &destination, double sampling_rate,
           std::error_code &err);
