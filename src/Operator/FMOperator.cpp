#include "FMOperator.h"

FMOperator::FMOperator(AudioProcessorValueTreeState* vst)
    : m_valueTreeState(vst),
    m_basePhase(0),
    m_feedbackGain(0),
    m_frequency(0) {
}

void FMOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_envelope.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_envelope.prepareToPlay(sampleRate, samplesPerBlock);
}

void FMOperator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    const float freqMultiplier = *m_valueTreeState->getRawParameterValue("freq_multiplier");
    const float envAttack = *m_valueTreeState->getRawParameterValue("env_attack");
    const float envAttackLevel = *m_valueTreeState->getRawParameterValue("env_attackLevel");
    const float envDecay = *m_valueTreeState->getRawParameterValue("env_decay");
    const float envSustain = *m_valueTreeState->getRawParameterValue("env_sustain");
    const float envRelease = *m_valueTreeState->getRawParameterValue("env_release");
    const int waveformIndex = *m_valueTreeState->getRawParameterValue("waveform");

    m_wavetable.setWavetable(waveformIndex);
    m_envelope.setParameters(envAttack, envAttackLevel, envDecay, envSustain, envRelease);
    double output = 0;
    double increment = m_frequency * freqMultiplier / getSampleRate();
    //double increment = m_frequency * freqMultiplier / getSampleRate();

    float* channelData = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_basePhase += increment;
        m_modulatedPhase = m_basePhase
                        + channelData[i]
                        + m_feedbackGain * output;
        output = m_wavetable(m_modulatedPhase);
        //output = sin(m_modulatedPhase);
        channelData[i] = output;
    }
    m_envelope.processBlock(buffer, midiMessages);
}

void FMOperator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    m_envelope.handleNoteOn(midiChannel, midiNoteNumber, velocity);
}

void FMOperator::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_envelope.handleNoteOff(midiChannel, midiNoteNumber, velocity);
}
