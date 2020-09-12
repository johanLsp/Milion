#include "ResonantFilter.h"

#define _USE_MATH_DEFINES
#include <cmath>

void ResonantFilter::prepareToPlay(double sampleRate, int samplesPerBlock) {
    setCenterFrequency(1000);
    setBandwidth(100);
    m_y[1] = 0;
    m_y[2] = 0;
}

void ResonantFilter::setCenterFrequency(double frequency) {
    m_centerFrequency = frequency;
    m_coef[1] = (m_coef[2] * 4.0) * cos(m_centerFrequency * (2*M_PI/getSampleRate()))/ (m_coef[2] + 1.0);
}

void ResonantFilter::setBandwidth(double bandwidth) {
    /* bw needs to stay positive so it doesn't blow the filter up */
    m_bandwidth = fabs(bandwidth);
    m_coef[2] = exp(m_bandwidth * (-2*M_PI/getSampleRate()));
    m_coef[1] = (m_coef[2] * 4.0) * cos(m_centerFrequency * (2*M_PI/getSampleRate()))/ (m_coef[2] + 1.0);
}

void ResonantFilter::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    float* channelData = buffer.getWritePointer(0);
    //float* frequencyData = buffer.getWritePointer(1);
    int sampleRate = static_cast<int>(getSampleRate());

    m_coef[1] = (m_coef[2] * 4.0) * cos(m_centerFrequency * (2*M_PI/sampleRate))/ (m_coef[2] + 1.0);
       
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_y[0] = m_coef[0] * channelData[i] + m_coef[1] * m_y[1] - m_coef[2] * m_y[2];
        channelData[i] = m_y[0] * m_centerFrequency / (4 * sampleRate);
        m_y[2] = m_y[1];
        m_y[1] = m_y[0];
    }
}
