#include "OperatorContainer.h"

OperatorContainer::OperatorContainer()
    : m_pOperator(nullptr) {
}

OperatorContainer::~OperatorContainer() {
    if (m_pOperator) delete m_pOperator;
}

void OperatorContainer::releaseResources() {
}

AudioProcessorEditor* OperatorContainer::createEditor() {
}

void OperatorContainer::getStateInformation(MemoryBlock& destData) {
}

void OperatorContainer::setStateInformation(const void* data, int sizeInBytes) {
}

void OperatorContainer::setValueTreeState(AudioProcessorValueTreeState* vst) {
    m_valueTreeState = vst;

    m_valueTreeState->createAndAddParameter("freq_multiplier",       // parameter ID
                                "Frequency Multiplier",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (1.0f, 10.0f),    // range
                                1.0f,         // default value
                                nullptr,
                                nullptr);
    m_valueTreeState->state = ValueTree(Identifier("Milion"));
}

void OperatorContainer::setOperator(Operator op) {
    if (m_pOperator) delete m_pOperator;

    switch (op) {
        case Operator::FM:
        m_pOperator = new FMOperator(m_valueTreeState);
    }
}

void OperatorContainer::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_pOperator->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_pOperator->prepareToPlay(sampleRate, samplesPerBlock);
}
void OperatorContainer::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_pOperator->processBlock(buffer, midiMessages);
}

void OperatorContainer::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_pOperator->handleNoteOn(midiChannel, midiNoteNumber, velocity);
}

void OperatorContainer::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_pOperator->handleNoteOff(midiChannel, midiNoteNumber, velocity);
}