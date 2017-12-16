#include "ImpulseGenerator.h"

ImpulseGenerator::ImpulseGenerator(double sampleRate)
: m_sampleRate(sampleRate), m_currentTick(0) {
    m_frequency = 1000;
}


void ImpulseGenerator::processBlock(AudioSampleBuffer& buffer) {
    int period = m_sampleRate / m_frequency;
    buffer.clear(0, 0, buffer.getNumSamples());
    float* channelData = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_currentTick++;
        if (m_currentTick % period) {
            channelData[i] = 1000;
        }
    }
}
