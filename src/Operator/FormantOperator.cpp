#include "FormantOperator.h"

FormantOperator::FormantOperator()
 : currentX(0), currentY(0),
    m_basePhase(0),
    m_frequency(0) {
    m_Xwave.setWavetable(0);
    m_Ywave.setWavetable(0);
}

void FormantOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    if(m_pBuffer) delete m_pBuffer;

    m_pBuffer = new AudioSampleBuffer(5, samplesPerBlock);

    Xinc = samplesPerBlock / (2 * sampleRate);
    Yinc = samplesPerBlock / (10 * sampleRate);

    m_envelope.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_envelope.prepareToPlay(sampleRate, samplesPerBlock);


}

void FormantOperator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    const float freqMultiplier = *m_valueTreeState->getRawParameterValue("freq_multiplier");
    const float envAttack = *m_valueTreeState->getRawParameterValue("env_attack");
    const float envAttackLevel = *m_valueTreeState->getRawParameterValue("env_attackLevel");
    const float envDecay = *m_valueTreeState->getRawParameterValue("env_decay");
    const float envSustain = *m_valueTreeState->getRawParameterValue("env_sustain");
    const float envRelease = *m_valueTreeState->getRawParameterValue("env_release");
    const int waveformIndex = static_cast<int>(*m_valueTreeState->getRawParameterValue("waveform"));
    const float bandwidth = *m_valueTreeState->getRawParameterValue("bandwidth");

    m_wavetable.setWavetable(waveformIndex);
    m_envelope.setParameters(envAttack, envAttackLevel, envDecay, envSustain, envRelease);


        currentX += Xinc;
        currentY += Yinc;
        double x = 0.5*m_Xwave(currentX) + 0.5;
        double y = 0.5*m_Ywave(currentY) + 0.5;


    for (int i = 0; i < 4; i++) {
        float freq1 = kPhonemeA[i] + x * (kPhonemeE[i] - kPhonemeA[i]);
        float freq2 = kPhonemeI[i] + x * (kPhonemeU[i] - kPhonemeI[i]);
        float frequency = freq1 + y * (freq2 - freq1);
        m_filters[i] = dsp::IIR::Filter<float>(dsp::IIR::Coefficients<float>::makeBandPass(getSampleRate(), frequency, bandwidth));
        //m_filters[i].reset();
    }

    for (int i = 0; i < 3; i++) {
            m_pBuffer->copyFrom(i, 0, buffer, 0, 0, buffer.getNumSamples());
    }

    dsp::AudioBlock<float> block = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    dsp::ProcessContextReplacing<float> context(block);
    m_filters[0].process(context);

    for(int i = 0; i < 3; i++) {
        block = dsp::AudioBlock<float>(*m_pBuffer).getSingleChannelBlock(i);
        dsp::ProcessContextReplacing<float> context(block);
        m_filters[i].process(context);
        buffer.addFrom(0, 0, *m_pBuffer, i, 0, buffer.getNumSamples());
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
