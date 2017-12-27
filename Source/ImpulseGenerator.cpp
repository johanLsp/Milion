#include "ImpulseGenerator.h"

ImpulseGenerator::ImpulseGenerator()
: m_currentTick(0) {
    m_frequency = 1000;
}



const String ImpulseGenerator::getName() const {
    return "Impulse Generator Processor";
}

void ImpulseGenerator::releaseResources() {
}

double ImpulseGenerator::getTailLengthSeconds() const {
    return 0.0;
}

bool ImpulseGenerator::acceptsMidi() const {
    return false;
}

bool ImpulseGenerator::producesMidi() const {
    return false;
}

AudioProcessorEditor* ImpulseGenerator::createEditor() {

}

bool ImpulseGenerator::hasEditor() const {
    return false;
}

int ImpulseGenerator::getNumPrograms() {
    return 1;
}

int ImpulseGenerator::getCurrentProgram() {
    return 0;
}

void ImpulseGenerator::setCurrentProgram(int index) {
}

const String ImpulseGenerator::getProgramName(int index) {
    return {};
}

void ImpulseGenerator::changeProgramName(int index, const String& newName) {
}

void ImpulseGenerator::getStateInformation(MemoryBlock& destData) {
}

void ImpulseGenerator::setStateInformation(const void* data, int sizeInBytes) {
}

void ImpulseGenerator::setFrequency(int frequency) {
    m_frequency = frequency;
}
void ImpulseGenerator::processBlock(AudioSampleBuffer& buffer) {
    int period = getSampleRate() / m_frequency;
    float* channelData = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_currentTick++;
        if (m_currentTick % period == 0) {
            channelData[i] = period - 1;
        } else {
            channelData[i] = -1.0;
        }
    }
}
