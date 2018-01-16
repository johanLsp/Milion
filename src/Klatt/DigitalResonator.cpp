#include "DigitalResonator.h"

DigitalResonator::DigitalResonator() {
}

DigitalResonator::~DigitalResonator() {
}

const String DigitalResonator::getName() const {
    return "Digital Resonator Processor";
}

void DigitalResonator::releaseResources() {
}

double DigitalResonator::getTailLengthSeconds() const {
    return 0.0;
}

bool DigitalResonator::acceptsMidi() const {
    return false;
}

bool DigitalResonator::producesMidi() const {
    return false;
}

AudioProcessorEditor* DigitalResonator::createEditor() {

}

bool DigitalResonator::hasEditor() const {
    return false;
}

int DigitalResonator::getNumPrograms() {
    return 1;
}

int DigitalResonator::getCurrentProgram() {
    return 0;
}

void DigitalResonator::setCurrentProgram(int index) {
}

const String DigitalResonator::getProgramName(int index) {
    return {};
}

void DigitalResonator::changeProgramName(int index, const String& newName) {
}

void DigitalResonator::getStateInformation(MemoryBlock& destData) {
}

void DigitalResonator::setStateInformation(const void* data, int sizeInBytes) {
}

void DigitalResonator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    setCenterFrequency(1000);
    setBandwidth(100);
    for (int channel = 0; channel < 2; channel++) {
        m_y[channel] = 0;
        m_yy[channel] = 0;
    }
}

void DigitalResonator::setCenterFrequency(double frequency) {
    m_centerFrequency = frequency;
    m_C = -exp(-2.0 * M_PI * m_bandwidth / getSampleRate());
    m_B = 2 * exp(-1.0 * M_PI * m_bandwidth / getSampleRate())
            * cos(2.0 * M_PI * m_centerFrequency / getSampleRate());
    m_A = 1 - m_B - m_C;
}

void DigitalResonator::setBandwidth(double bandwidth) {
    m_bandwidth = bandwidth;
    m_C = -exp(-2.0 * M_PI * m_bandwidth / getSampleRate());
    m_B = 2 * exp(-1.0 * M_PI * m_bandwidth / getSampleRate())
            * cos(2.0 * M_PI * m_centerFrequency / getSampleRate());
    m_A = 1 - m_B - m_C;
}

void DigitalResonator::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {
    for(int channel = 0; channel < getTotalNumInputChannels(); channel++) {
        float* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            channelData[i] = m_A * channelData[i]
                        + m_B * m_y[channel]
                        + m_C * m_yy[channel];
            m_yy[channel] = m_y[channel];
            m_y[channel] = channelData[i];
        }
    }
}
