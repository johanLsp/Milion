#include "FormantOperator.h"

#include <memory>

using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
using NodeID =  AudioProcessorGraph::NodeID;

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
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode), NodeID(1));
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode), NodeID(2));

    for (int i = 0; i < 4; i++) {
        m_filters[i] = new ResonantFilter();
        m_graph.addNode(std::unique_ptr<AudioProcessor>(m_filters[i]), NodeID(i+3));
    }

    for (unsigned int i = 0; i < 4; i++) {
        m_filters[i]->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
        m_graph.addConnection({ {NodeID(1), 0}, {NodeID(i+3), 0} });
        m_graph.addConnection({ {NodeID(i+3), 0}, {NodeID(2), 0} });
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
