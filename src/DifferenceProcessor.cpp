#include "DifferenceProcessor.h"


DifferenceProcessor::DifferenceProcessor()
:m_sign(1) {
}

DifferenceProcessor::~DifferenceProcessor() {
}

const String DifferenceProcessor::getName() const {
    return "Difference Processor";
}

void DifferenceProcessor::releaseResources() {
}

double DifferenceProcessor::getTailLengthSeconds() const {
    return 0.0;
}

bool DifferenceProcessor::acceptsMidi() const {
    return false;
}

bool DifferenceProcessor::producesMidi() const {
    return false;
}

AudioProcessorEditor* DifferenceProcessor::createEditor() {

}

bool DifferenceProcessor::hasEditor() const {
    return false;
}

int DifferenceProcessor::getNumPrograms() {
    return 1;
}

int DifferenceProcessor::getCurrentProgram() {
    return 0;
}

void DifferenceProcessor::setCurrentProgram(int index) {
}

const String DifferenceProcessor::getProgramName(int index) {
    return {};
}

void DifferenceProcessor::changeProgramName(int index, const String& newName) {
}

void DifferenceProcessor::getStateInformation(MemoryBlock& destData) {
}

void DifferenceProcessor::setStateInformation(const void* data, int sizeInBytes) {
}

void DifferenceProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    for (int channel = 0; channel < 2; channel++) {
        m_xx[channel] = 0;
    }
}

void DifferenceProcessor::setType(DifferenceProcessor::Type type) {
    if (type == DifferenceProcessor::Type::LOWPASS) {
        m_sign = 1;
    } else {
        m_sign = -1;
    }
}

void DifferenceProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    for (int channel = 0; channel < getTotalNumInputChannels(); channel++) {
        float* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            double x = channelData[i];
            channelData[i] = x + m_sign * m_xx[channel];
            m_xx[channel] = x;
        }
    }
}
