#include "DifferenceProcessor.h"

#include <cmath>

void DifferenceProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    for (int channel = 0; channel < 2; channel++) {
        m_xx[channel] = 0;
    }
}

void DifferenceProcessor::setType(DifferenceProcessor::Type type) {
    if (type == DifferenceProcessor::Type::LOWPASS) {
        m_sign = 1;
    } else {
        m_sign = -1;
    }
}

void DifferenceProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    for (int channel = 0; channel < getTotalNumInputChannels(); channel++) {
        float* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            double x = channelData[i];
            channelData[i] = x + m_sign * m_xx[channel];
            m_xx[channel] = x;
        }
    }
}
