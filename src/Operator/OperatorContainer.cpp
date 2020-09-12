#include "OperatorContainer.h"

OperatorContainer::OperatorContainer() {
    m_operators[0] = new FMOperator();
    m_operators[1] = new FormantOperator();
}

OperatorContainer::~OperatorContainer() {
    delete m_operators[0];
    delete m_operators[1];
}

void OperatorContainer::releaseResources() {
}

void OperatorContainer::getStateInformation(MemoryBlock& destData) {
}

void OperatorContainer::setStateInformation(const void* data, int sizeInBytes) {
}

void OperatorContainer::setValueTreeState(AudioProcessorValueTreeState* vst) {
    m_valueTreeState = vst;
    m_operators[0]->setValueTreeState(vst);
    m_operators[1]->setValueTreeState(vst);

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "freq_multiplier", "Frequency", NormalisableRange<float> (1.0f, 10.0f, 0.01f, 0.5f), 1.0f));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "env_attack", "Envelope attack time", NormalisableRange<float> (0.01f, 1.0f), 0.2f));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "env_attackLevel", "Envelope attack level", NormalisableRange<float> (0.01f, 1.0f), 0.2f));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "env_decay", "Envelope decay time", NormalisableRange<float> (0.01f, 1.0f), 0.2f));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "env_sustain", "Envelope sustain time", NormalisableRange<float> (0.01f, 1.0f), 0.2f));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "env_release", "Envelope release time", NormalisableRange<float> (0.01f, 1.0f), 0.2f));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "waveform", "Envelope sustain time", NormalisableRange<float> (1, 7), 1));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "bandwidth", "Bandwidth", NormalisableRange<float> (0.01f, 50.0f), 1.0f));

    m_valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat> (
        "skirt", "Skirt", NormalisableRange<float> (0.01f, 10.0f), 0.2f));
}

void OperatorContainer::setOperator(Operator op) {
    m_pCurrentOperator = m_operators[op];
    prepareToPlay(getSampleRate(), getBlockSize());
}

void OperatorContainer::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_pCurrentOperator->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_pCurrentOperator->prepareToPlay(sampleRate, samplesPerBlock);
}
void OperatorContainer::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_pCurrentOperator->processBlock(buffer, midiMessages);
}

void OperatorContainer::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_pCurrentOperator->handleNoteOn(midiChannel, midiNoteNumber, velocity);
}

void OperatorContainer::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_pCurrentOperator->handleNoteOff(midiChannel, midiNoteNumber, velocity);
}
