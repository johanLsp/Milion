#include "GainProcessor.h"

void GainProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_gain = 1.0;
}

void GainProcessor::setGain(double gain) {
    m_gain = gain;
}

void GainProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    buffer.applyGain(m_gain);
}
