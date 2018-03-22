#include "FormantOperator.h"


FormantOperator::FormantOperator()
 : currentX(0), currentY(0),
    m_basePhase(0),
    m_frequency(0) {
    m_Xwave.setWavetable(6);
    m_Ywave.setWavetable(6);

    file.open("test.dat");

}

void FormantOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    
    for(int i = 0; i < 3 ; i++) {
        if(m_pBuffer[i]) delete m_pBuffer[i];
        m_pBuffer[i] = new AudioSampleBuffer(1, samplesPerBlock);
    }


    Xinc = samplesPerBlock / (2 * sampleRate);
    Yinc = samplesPerBlock / (10 * sampleRate);

    m_envelope.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_envelope.prepareToPlay(sampleRate, samplesPerBlock);

    for (int i = 0; i < 4; i++) {
        m_resonant[i].setPlayConfigDetails(
            getTotalNumInputChannels(),
            getTotalNumOutputChannels(),
            sampleRate,
            samplesPerBlock);
        m_resonant[i].prepareToPlay(sampleRate, samplesPerBlock);
    }

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
    const float skirt = *m_valueTreeState->getRawParameterValue("skirt");

    m_wavetable.setWavetable(waveformIndex);
    m_envelope.setParameters(envAttack, envAttackLevel, envDecay, envSustain, envRelease);


    m_lowPass.coefficients = dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 100*skirt, bandwidth);

    currentX += Xinc;
    currentY += Yinc;
    double x = 0.5*m_Xwave(currentX) + 0.5;
    double y = 0.5*m_Ywave(currentY) + 0.5;

    //x = 1;
    //y = 0;

    for (int i = 0; i < 4; i++) {
        float freq1 = kPhonemeA[i] + x * (kPhonemeE[i] - kPhonemeA[i]);
        float freq2 = kPhonemeI[i] + x * (kPhonemeU[i] - kPhonemeI[i]);
        float frequency = freq1 + y * (freq2 - freq1);

        m_resonant[i].setCenterFrequency(frequency);
        m_resonant[i].setBandwidth(0.02*frequency+50);
        //m_filters[i].coefficients = dsp::IIR::Coefficients<float>::makeBandPass(getSampleRate(), frequency, frequency / (0.02*frequency+50));
        m_filters[i].coefficients = dsp::IIR::Coefficients<float>::makeBandPass(getSampleRate(), frequency, bandwidth);
        //m_filters[i].coefficients = dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), frequency, bandwidth, skirt);
    }




    for (int i = 0; i < 3; i++) {
            m_pBuffer[i]->copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            m_resonant[i+1].processBlock(*m_pBuffer[i], midiMessages);
    }

    m_resonant[0].processBlock(buffer, midiMessages);

    buffer.applyGain(0, 0, buffer.getNumSamples(), 0.25);

    for (int i = 0; i < 3; i++) {
        buffer.addFrom(0, 0, *m_pBuffer[i], 0, 0, buffer.getNumSamples(), 0.25);
    }
    
         //   buffer.copyFrom(0, 0, *m_pBuffer[1], 0, 0, buffer.getNumSamples());


/*
    dsp::AudioBlock<float> block = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    dsp::ProcessContextReplacing<float> context(block);
    m_filters[0].process(context);
    //buffer.applyGain(0, 0, buffer.getNumSamples(), 2);

    for(int i = 0; i < 3; i++) {
        block = dsp::AudioBlock<float>(*m_pBuffer).getSingleChannelBlock(i);
        dsp::ProcessContextReplacing<float> context(block);
        m_filters[i].process(context);
        //buffer.addFrom(0, 0, *m_pBuffer, i, 0, buffer.getNumSamples());
    }

    //buffer.applyGain(0, 0, buffer.getNumSamples(), 2);

    const float* data = buffer.getReadPointer(0);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
       // file << data[i] << std::endl;
    }

    */

    dsp::AudioBlock<float> block = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    dsp::ProcessContextReplacing<float> context2(block);
    //m_lowPass.process(context2);
    //m_envelope.processBlock(buffer, midiMessages);
}

void FormantOperator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    m_envelope.handleNoteOn(midiChannel, midiNoteNumber, velocity);
}

void FormantOperator::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_envelope.handleNoteOff(midiChannel, midiNoteNumber, velocity);
}
