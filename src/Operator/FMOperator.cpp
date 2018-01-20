#include "FMOperator.h"

FMOperator::FMOperator(AudioProcessorValueTreeState& vst)
    :BaseOperator(vst) {
    m_parameters.createAndAddParameter("freq_multiplier",       // parameter ID
                                  "Frequency Multiplier",       // parameter name
                                  String(),     // parameter label (suffix)
                                  NormalisableRange<float> (1.0f, 10.0f),    // range
                                  1.0f,         // default value
                                  nullptr,
                                  nullptr);

}

void FMOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_basePhase = 0;
    m_feedbackGain = 0;
    m_frequency = 0;
    m_frequencyMultiplier = 1;
}
void FMOperator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    const float freqMultiplier = *m_parameters.getRawParameterValue ("freq_multiplier");

    double output = 0;
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        m_basePhase += m_frequency * freqMultiplier / getSampleRate();

        for (int channel = 0; channel < getTotalNumInputChannels(); channel++) {
            float* channelData = buffer.getWritePointer(channel);
            m_modulatedPhase = m_basePhase
                             + channelData[i]
                             + m_feedbackGain * output;
            output = sin(m_modulatedPhase);
            channelData[i] = output;
        }
    }
}

void FMOperator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}

void FMOperator::setFrequencyMultiplier(double multiplier) {
    m_frequencyMultiplier = multiplier;
}

