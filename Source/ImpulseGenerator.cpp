#include "ImpulseGenerator.h"

ImpulseGenerator::ImpulseGenerator(int sampleRate)
: m_sampleRate(sampleRate), m_currentTick(0) {
    m_frequency = 1000;
}

void ImpulseGenerator::setFrequency(int frequency) {
    m_frequency = frequency;
}
void ImpulseGenerator::processBlock(AudioSampleBuffer& buffer) {
    int period = m_sampleRate / m_frequency;
    float* channelData = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_currentTick++;
        if (m_currentTick % period == 0) {
            channelData[i] = period - 1;
        } else {
            channelData[i] = -1.0;
        }
    }
}
