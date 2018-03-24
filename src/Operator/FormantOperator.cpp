#include "FormantOperator.h"

FormantOperator::FormantOperator() {
}

void FormantOperator::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_graph.setPlayConfigDetails(
        getTotalNumInputChannels(),
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

    for (unsigned int i = 0; i < 4; i++) {
        m_filters[i]->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
        m_graph.addConnection({ {1, 0}, {i+3, 0} });
        m_graph.addConnection({ {i+3, 0}, {2, 0} });
    }

    m_graph.prepareToPlay(sampleRate, samplesPerBlock);
}

void FormantOperator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    const float x = *m_valueTreeState->getRawParameterValue("bandwidth");
    const float y = *m_valueTreeState->getRawParameterValue("skirt");

    for (int i = 0; i < 4; i++) {
        float freq1 = kPhonemeA[i] + x * (kPhonemeE[i] - kPhonemeA[i]);
        float freq2 = kPhonemeI[i] + x * (kPhonemeU[i] - kPhonemeI[i]);
        float frequency = freq1 + y * (freq2 - freq1);
        m_filters[i]->setCenterFrequency(frequency);
        m_filters[i]->setBandwidth(0.02*frequency+50);
    }
    m_graph.processBlock(buffer, midiMessages);
}
