#include "AntiResonator.h"

#include <cmath>

void AntiResonator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    setCenterFrequency(1000);
    setBandwidth(100);
    for (int channel = 0; channel < 2; channel++) {
        m_x[channel] = 0;
        m_xx[channel] = 0;
    }
}

void AntiResonator::setCenterFrequency(double frequency) {
    m_centerFrequency = frequency;
    m_C = -exp(-2.0 * M_PI * m_bandwidth / getSampleRate());
    m_B = 2 * exp(-1.0 * M_PI * m_bandwidth / getSampleRate())
            * cos(2.0 * M_PI * m_centerFrequency / getSampleRate());
    m_A = 1 - m_B - m_C;

    m_C = -m_C / m_A;
    m_B = -m_B / m_A;
    m_A = 1 / m_A;
}

void AntiResonator::setBandwidth(double bandwidth) {
    m_bandwidth = bandwidth;
    m_C = -exp(-2.0 * M_PI * m_bandwidth / getSampleRate());
    m_B = 2 * exp(-1.0 * M_PI * m_bandwidth / getSampleRate())
            * cos(2.0 * M_PI * m_centerFrequency / getSampleRate());
    m_A = 1 - m_B - m_C;

    m_C = -m_C / m_A;
    m_B = -m_B / m_A;
    m_A = 1 / m_A;
}

void AntiResonator::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    for (int channel = 0; channel < getTotalNumInputChannels(); channel++) {
        float* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            double input = channelData[i];
            channelData[i] = m_A * input
                        + m_B * m_x[channel]
                        + m_C * m_xx[channel];
            m_xx[channel] = m_x[channel];
            m_x[channel] = input;
        }
    }
}
