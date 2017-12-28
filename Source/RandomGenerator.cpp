#include "RandomGenerator.h"

RandomGenerator::RandomGenerator()
: m_currentTick(0) {
    m_frequency = 1000;
}

RandomGenerator::~RandomGenerator() {
}

const String RandomGenerator::getName() const {
    return "Random Generator Processor";
}

void RandomGenerator::releaseResources() {
}

double RandomGenerator::getTailLengthSeconds() const {
    return 0.0;
}

bool RandomGenerator::acceptsMidi() const {
    return false;
}

bool RandomGenerator::producesMidi() const {
    return false;
}

AudioProcessorEditor* RandomGenerator::createEditor() {
}

bool RandomGenerator::hasEditor() const {
    return false;
}

int RandomGenerator::getNumPrograms() {
    return 1;
}

int RandomGenerator::getCurrentProgram() {
    return 0;
}

void RandomGenerator::setCurrentProgram(int index) {
}

const String RandomGenerator::getProgramName(int index) {
    return {};
}

void RandomGenerator::changeProgramName(int index, const String& newName) {
}

void RandomGenerator::getStateInformation(MemoryBlock& destData) {
}

void RandomGenerator::setStateInformation(const void* data, int sizeInBytes) {
}

void RandomGenerator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_random.setSeedRandomly();
}

void RandomGenerator::setFrequency(int frequency) {
    m_frequency = frequency;
}

void RandomGenerator::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    double period = getSampleRate() / m_frequency;
    float* channelData = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_currentTick++;
        channelData[i] = (2 * m_random.nextDouble() - 1) * cos(2 * M_PI * m_currentTick / period);
    }
}
