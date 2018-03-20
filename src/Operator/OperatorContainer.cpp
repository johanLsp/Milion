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

    m_valueTreeState->createAndAddParameter("freq_multiplier",       // parameter ID
                                "Frequency",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (1.0f, 10.0f, 0.01f, 0.5f),    // range
                                1.0f,         // default value
                                nullptr,
                                nullptr);

    m_valueTreeState->createAndAddParameter("env_attack",       // parameter ID
                                "Envelope attack time",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (0.01f, 1.0f),    // range
                                0.2f,         // default value
                                nullptr,
                                nullptr);

    m_valueTreeState->createAndAddParameter("env_attackLevel",       // parameter ID
                                "Envelope attackLevel time",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (0.01f, 1.0f),    // range
                                0.2f,         // default value
                                nullptr,
                                nullptr);

    m_valueTreeState->createAndAddParameter("env_decay",       // parameter ID
                                "Envelope decay time",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (0.01f, 1.0f),    // range
                                0.2f,         // default value
                                nullptr,
                                nullptr);

    m_valueTreeState->createAndAddParameter("env_sustain",       // parameter ID
                                "Envelope sustain time",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (0.01f, 1.0f),    // range
                                0.2f,         // default value
                                nullptr,
                                nullptr);

    m_valueTreeState->createAndAddParameter("env_release",       // parameter ID
                                "Envelope release time",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (0.01f, 1.0f),    // range
                                0.2f,         // default value
                                nullptr,
                                nullptr);
    m_valueTreeState->createAndAddParameter("waveform",       // parameter ID
                                "Waveform selection",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (1, 7),    // range
                                0.2f,         // default value
                                nullptr,
                                nullptr);

    m_valueTreeState->createAndAddParameter("bandwidth",       // parameter ID
                                "Bandwidth",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (0.1f, 50.0f),    // range
                                1.0f,         // default value
                                nullptr,
                                nullptr);
                                    m_valueTreeState->createAndAddParameter("skirt",       // parameter ID
                                "Skirt",       // parameter name
                                String(),     // parameter label (suffix)
                                NormalisableRange<float> (1.0f, 10.0f),    // range
                                0.2f,         // default value
                                nullptr,
                                nullptr);
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
