#include "GainProcessor.h"

GainProcessor::GainProcessor() {
}

GainProcessor::~GainProcessor() {
}

const String GainProcessor::getName() const {
    return "Gain Processor";
}

void GainProcessor::releaseResources() {
}

double GainProcessor::getTailLengthSeconds() const {
    return 0.0;
}

bool GainProcessor::acceptsMidi() const {
    return false;
}

bool GainProcessor::producesMidi() const {
    return false;
}

AudioProcessorEditor* GainProcessor::createEditor() {
}

bool GainProcessor::hasEditor() const {
    return false;
}

int GainProcessor::getNumPrograms() {
    return 1;
}

int GainProcessor::getCurrentProgram() {
    return 0;
}

void GainProcessor::setCurrentProgram(int index) {
}

const String GainProcessor::getProgramName(int index) {
    return {};
}

void GainProcessor::changeProgramName(int index, const String& newName) {
}

void GainProcessor::getStateInformation(MemoryBlock& destData) {
}

void GainProcessor::setStateInformation(const void* data, int sizeInBytes) {
}

void GainProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
}

void GainProcessor::setGain(double gain) {
    m_gain = gain;
}

void GainProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    buffer.applyGain(m_gain);
}
