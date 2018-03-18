#include "FormantOperator.h"

FormantOperator::FormantOperator()
 : currentX(0), currentY(0),
    m_basePhase(0),
    m_frequency(0) {
    m_Xwave.setWavetable(6);
    m_Ywave.setWavetable(6);
}

void FormantOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    if(m_pBuffer) delete m_pBuffer;

    m_pBuffer = new AudioSampleBuffer(5, samplesPerBlock);

    m_graph.setPlayConfigDetails(
        5 * getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);

    m_graph.clear();

    AudioProcessorGraph::AudioGraphIOProcessor* input =
        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);

    AudioProcessorGraph::AudioGraphIOProcessor* output =
        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);

    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);


    for (int i = 0; i < 4; i++) {
        m_filters[i] = new ResonantFilter();
        m_graph.addNode(m_filters[i], i+3);
    }

    for (int i = m_graph.getNumNodes() - 1; i >= 0; i--)
        m_graph.disconnectNode(i);

    for (int i = 0; i < 4; i++) {
        m_filters[i]->setPlayConfigDetails(
        2 * getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
        m_graph.addConnection({ {1, 0}, {i+3, 0} });
        m_graph.addConnection({ {1, i+1}, {i+3, 1} });
        m_graph.addConnection({ {i+3, 0}, {2, 0} });
    }

    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

    Xinc =  1.0 / (2 * sampleRate);
    Yinc = 1.0 / (10 * sampleRate);

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

    m_wavetable.setWavetable(waveformIndex);
    m_envelope.setParameters(envAttack, envAttackLevel, envDecay, envSustain, envRelease);
    double increment = m_frequency * freqMultiplier / getSampleRate();

    float* inputData = buffer.getWritePointer(0);
    float* audioData = m_pBuffer->getWritePointer(0);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        currentX += Xinc;
        currentY += Yinc;
        double x = m_Xwave(currentX);
        double y = m_Ywave(currentY);

        for (int channel = 0; channel < 4; channel++) {
            float* frequencyData = m_pBuffer->getWritePointer(channel+1);
            double freq1 = kPhonemeA[channel] + x * (kPhonemeE[channel] - kPhonemeA[channel]);
            double freq2 = kPhonemeI[channel] + x * (kPhonemeU[channel] - kPhonemeI[channel]);
            double frequency = freq1 + y * (freq2 - freq1);
            frequencyData[i] = frequency;
        }

        m_basePhase += increment;
        m_modulatedPhase = m_basePhase + inputData[i];
        audioData[i] = m_wavetable(m_modulatedPhase);
    }

    m_graph.processBlock(*m_pBuffer, midiMessages);
    buffer.copyFrom(0, 0, *m_pBuffer, 0, 0, buffer.getNumSamples());
    m_envelope.processBlock(buffer, midiMessages);
}

void FormantOperator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    m_envelope.handleNoteOn(midiChannel, midiNoteNumber, velocity);
}

void FormantOperator::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_envelope.handleNoteOff(midiChannel, midiNoteNumber, velocity);
}
