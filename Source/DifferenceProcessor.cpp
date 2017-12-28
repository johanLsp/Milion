#include "DifferenceProcessor.h"

DifferenceProcessor::DifferenceProcessor() {
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
    m_xx = 0;
}

void DifferenceProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    float* channelData = buffer.getWritePointer(0);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        double x = channelData[i];
        channelData[i] = x - m_xx;
        m_xx = x;
    }
}
