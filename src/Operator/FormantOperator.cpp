#include "FormantOperator.h"

FormantOperator::FormantOperator(AudioProcessorValueTreeState* vst)
    : m_valueTreeState(vst),
    m_frequency(440),
    m_mainPhasor {0,0},
    m_secondaryPhasor {0,0},
    m_bandwidthPhasor {0,0} {
    
    m_mainPhasor[0] = 0;
    m_mainPhasor[1] = 0.5;
    m_secondaryPhasor[0] = 0;
    m_secondaryPhasor[1] = 0.5;
    m_bandwidthPhasor[0] = 0;
    m_bandwidthPhasor[1] = 0.5;
}

void FormantOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_envelope.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_envelope.prepareToPlay(sampleRate, samplesPerBlock);

    m_window[0].setWavetable(1);
    m_window[1].setWavetable(1);
}

void FormantOperator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    const float freqMultiplier = *m_valueTreeState->getRawParameterValue("freq_multiplier");
    const float envAttack = *m_valueTreeState->getRawParameterValue("env_attack");
    const float envAttackLevel = *m_valueTreeState->getRawParameterValue("env_attackLevel");
    const float envDecay = *m_valueTreeState->getRawParameterValue("env_decay");
    const float envSustain = *m_valueTreeState->getRawParameterValue("env_sustain");
    const float envRelease = *m_valueTreeState->getRawParameterValue("env_release");
    const int waveformIndex = *m_valueTreeState->getRawParameterValue("waveform");
    const float bandwidth = *m_valueTreeState->getRawParameterValue("bandwidth");
    const float skirt = *m_valueTreeState->getRawParameterValue("skirt");

    m_wavetable[0].setWavetable(waveformIndex);
    m_wavetable[1].setWavetable(waveformIndex);

    m_envelope.setParameters(envAttack, envAttackLevel, envDecay, envSustain, envRelease);

    double output = 0;
    double mainInc = m_frequency * freqMultiplier / getSampleRate();
    double secondaryInc =  m_frequency / getSampleRate() / 2;
    double bandwidthInc = m_frequency / bandwidth / getSampleRate();
    double windowPhasor[2];
    double modulatedPhasor[2];
    float* channelData = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        for (int layer = 0; layer < 2; layer++) {
            m_mainPhasor[layer] += mainInc;
            m_secondaryPhasor[layer] += secondaryInc;
            m_bandwidthPhasor[layer] += bandwidthInc;
            if (m_bandwidthPhasor[layer] > 1)  m_bandwidthPhasor[layer] = 1;
            if (m_secondaryPhasor[layer] >= 1) {
                m_mainPhasor[layer] = 0;
                m_secondaryPhasor[layer] = 0;
                m_bandwidthPhasor[layer] = 0;
            }
            if (m_secondaryPhasor[layer] >= m_bandwidthPhasor[layer])
                windowPhasor[layer] = m_secondaryPhasor[layer] + channelData[i];
            else
                windowPhasor[layer] = m_bandwidthPhasor[layer] + channelData[i];

            modulatedPhasor[layer] = m_mainPhasor[layer] + channelData[i];
        }
       channelData[i] = m_wavetable[0](modulatedPhasor[0])*pow(sin(M_PI * windowPhasor[0]) , 2*skirt)
        + m_wavetable[1](modulatedPhasor[1])*pow(sin(M_PI * windowPhasor[1]) , 2*skirt);
   }
    m_envelope.processBlock(buffer, midiMessages);
}

void FormantOperator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    m_envelope.handleNoteOn(midiChannel, midiNoteNumber, velocity);
}

void FormantOperator::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_envelope.handleNoteOff(midiChannel, midiNoteNumber, velocity);
}
