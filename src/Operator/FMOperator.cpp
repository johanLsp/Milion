#include "FMOperator.h"

FMOperator::FMOperator(AudioProcessorValueTreeState* vst)
    : m_valueTreeState(vst),
    m_basePhase(0),
    m_feedbackGain(0),
    m_frequency(0) {
}

void FMOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_enveloppe.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_enveloppe.prepareToPlay(sampleRate, samplesPerBlock);
}

void FMOperator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    const float freqMultiplier = *m_valueTreeState->getRawParameterValue("freq_multiplier");

    m_enveloppe.setParameters(1.0, 1.0, 1.0, 0.5, 1.0);
    double output = 0;
    double increment = m_frequency * freqMultiplier / getSampleRate();
    float* channelData = buffer.getWritePointer(0);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_basePhase += increment;
        m_modulatedPhase = m_basePhase
                        + channelData[i]
                        + m_feedbackGain * output;
        output = sin(m_modulatedPhase);
        channelData[i] = output;
    }
    m_enveloppe.processBlock(buffer, midiMessages);
}

void FMOperator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    m_enveloppe.handleNoteOn(midiChannel, midiNoteNumber, velocity);
}

void FMOperator::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_enveloppe.handleNoteOff(midiChannel, midiNoteNumber, velocity);
}
