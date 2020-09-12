#include "ImpulseGenerator.h"

void ImpulseGenerator::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    int period = getSampleRate() / m_frequency;

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_currentTick++;
        for (int channel = 0; channel < getTotalNumInputChannels(); channel++) {
            float* channelData = buffer.getWritePointer(channel);
            if (m_currentTick % period == 0) {
                channelData[i] = period - 1;
            } else {
                channelData[i] = -1.0;
            }
        }
    }
}
