#include "AntiResonator.h"

AntiResonator::AntiResonator() {
}

AntiResonator::~AntiResonator() {
}

const String AntiResonator::getName() const {
    return "Anti Resonator Processor";
}

void AntiResonator::releaseResources() {
}

double AntiResonator::getTailLengthSeconds() const {
    return 0.0;
}

bool AntiResonator::acceptsMidi() const {
    return false;
}

bool AntiResonator::producesMidi() const {
    return false;
}

AudioProcessorEditor* AntiResonator::createEditor() {

}

bool AntiResonator::hasEditor() const {
    return false;
}

int AntiResonator::getNumPrograms() {
    return 1;
}

int AntiResonator::getCurrentProgram() {
    return 0;
}

void AntiResonator::setCurrentProgram(int index) {
}

const String AntiResonator::getProgramName(int index) {
    return {};
}

void AntiResonator::changeProgramName(int index, const String& newName) {
}

void AntiResonator::getStateInformation(MemoryBlock& destData) {
}

void AntiResonator::setStateInformation(const void* data, int sizeInBytes) {
}

void AntiResonator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    setCenterFrequency(1000);
    setBandwidth(100);
    for (int channel = 0; channel < 2; channel++) {
        m_x[channel] = 0;
        m_xx[channel] = 0;
    }
}

void AntiResonator::setCenterFrequency(double frequency) {
    m_centerFrequency = frequency;
    m_C = -exp(-2.0 * M_PI * m_bandwidth / getSampleRate());
    m_B = 2 * exp(-1.0 * M_PI * m_bandwidth / getSampleRate())
            * cos(2.0 * M_PI * m_centerFrequency / getSampleRate());
    m_A = 1 - m_B - m_C;

    m_C = -m_C / m_A;
    m_B = -m_B / m_A;
    m_A = 1 / m_A;
}

void AntiResonator::setBandwidth(double bandwidth) {
    m_bandwidth = bandwidth;
    m_C = -exp(-2.0 * M_PI * m_bandwidth / getSampleRate());
    m_B = 2 * exp(-1.0 * M_PI * m_bandwidth / getSampleRate())
            * cos(2.0 * M_PI * m_centerFrequency / getSampleRate());
    m_A = 1 - m_B - m_C;

    m_C = -m_C / m_A;
    m_B = -m_B / m_A;
    m_A = 1 / m_A;
}

void AntiResonator::processBlock(AudioSampleBuffer& buffer, MidiBuffer &midiMessages) {

    for (int channel = 0; channel < getTotalNumInputChannels(); channel++) {
        float* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            double input = channelData[i];
            channelData[i] = m_A * input
                        + m_B * m_x[channel]
                        + m_C * m_xx[channel];
            m_xx[channel] = m_x[channel];
            m_x[channel] = input;
        }
    }
}
