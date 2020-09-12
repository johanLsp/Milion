#include "RandomGenerator.h"

void RandomGenerator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_random.setSeedRandomly();
}

void RandomGenerator::setFrequency(int frequency) {
    m_frequency = frequency;
}

void RandomGenerator::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    double period = getSampleRate() / m_frequency;
    float* channelData = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_currentTick++;
        for (int channel = 0; channel < getTotalNumInputChannels(); channel++) {
            float* channelData = buffer.getWritePointer(channel);
            channelData[i] = (2 * m_random.nextDouble() - 1) * cos(2 * M_PI * m_currentTick / period);
        }
    }
}
