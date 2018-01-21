#include "OperatorContainer.h"

OperatorContainer::OperatorContainer() {
}

OperatorContainer::~OperatorContainer() {
    if (m_pOperator) delete m_pOperator;
}

const String OperatorContainer::getName() const {
    return "";
}

void OperatorContainer::releaseResources() {
}

double OperatorContainer::getTailLengthSeconds() const {
    return 0.0;
}

bool OperatorContainer::acceptsMidi() const {
    return false;
}

bool OperatorContainer::producesMidi() const {
    return false;
}

AudioProcessorEditor* OperatorContainer::createEditor() {
}

bool OperatorContainer::hasEditor() const {
    return false;
}

int OperatorContainer::getNumPrograms() {
    return 1;
}

int OperatorContainer::getCurrentProgram() {
    return 0;
}

void OperatorContainer::setCurrentProgram(int index) {
}

const String OperatorContainer::getProgramName(int index) {
    return {};
}

void OperatorContainer::changeProgramName(int index, const String& newName) {
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
