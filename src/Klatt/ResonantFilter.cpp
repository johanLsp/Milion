#include "ResonantFilter.h"
#include <sstream>

ResonantFilter::ResonantFilter()
 : coef1(1.0) {
}

ResonantFilter::~ResonantFilter() {
}

void ResonantFilter::prepareToPlay(double sampleRate, int samplesPerBlock) {
    setCenterFrequency(1000);
    setBandwidth(100);
    y1 = 0;
    y2 = 0;
}

void ResonantFilter::setCenterFrequency(double frequency) {
    m_centerFrequency;
    coef2= (coef3 * 4.0) * cos(m_centerFrequency * (2*M_PI/getSampleRate()))/ (coef3 + 1.0);
}

void ResonantFilter::setBandwidth(double bandwidth) {
    /* bw needs to stay positive so it doesn't blow the filter up */
    m_bandwidth = fabs(bandwidth);
    coef3= exp(bandwidth * (-2*M_PI/getSampleRate()));
    coef2= (coef3 * 4.0) * cos(m_centerFrequency * (2*M_PI/getSampleRate()))/ (coef3 + 1.0);
}

void ResonantFilter::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    float* channelData = buffer.getWritePointer(0);
    float* frequencyData = buffer.getWritePointer(1);
    int sampleRate = getSampleRate();

    coef2 = (coef3 * 4.0) * cos(m_centerFrequency * (2*M_PI/sampleRate))/ (coef3 + 1.0);
       
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        if (i % (sampleRate/10) == 0) {
            m_smoothedFrequency.setValue(frequencyData[i]);
            m_centerFrequency = m_smoothedFrequency.getNextValue();
            m_bandwidth = m_centerFrequency * 0.02 + 50.0;
            coef3 = exp(m_bandwidth * (-2*M_PI/sampleRate));
            }

        y0 = coef1 * channelData[i] + coef2 * y1 - coef3 * y2;
        channelData[i] = y0 * m_centerFrequency / (4 * sampleRate);
        y2 = y1;
        y1 = y0;
    }
}
