#include "CascadeVocal.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using NodeID =  AudioProcessorGraph::NodeID;

void CascadeVocal::prepareToPlay(double sampleRate, int samplesPerBlock) {

    m_graph.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);
    m_graph.prepareToPlay(sampleRate, samplesPerBlock);
    // Add input and output nodes.
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode), NodeID(1));
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode), NodeID(2));

    m_RNP = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_RNP), NodeID(3));

    m_RNZ = new AntiResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_RNZ), NodeID(4));

    m_R1 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R1), NodeID(5));

    m_R2 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R2), NodeID(6));

    m_R3 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R3), NodeID(7));

    m_R4 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R4), NodeID(8));

    m_R5 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R5), NodeID(9));

    m_RNP->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_RNP->setCenterFrequency(250);
    m_RNP->setBandwidth(100);

    m_RNZ->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_RNZ->setCenterFrequency(250);
    m_RNZ->setBandwidth(100);

    m_R1->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R1->setCenterFrequency(450);
    m_R1->setBandwidth(50);

    m_R2->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R2->setCenterFrequency(1450);
    m_R2->setBandwidth(70);

    m_R3->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R3->setCenterFrequency(2450);
    m_R3->setBandwidth(110);

    m_R4->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R4->setCenterFrequency(3300);
    m_R4->setBandwidth(250);

    m_R5->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R5->setCenterFrequency(3750);
    m_R5->setBandwidth(200);

    // Input -> RNP
    m_graph.addConnection({{NodeID(1), 0}, {NodeID(3), 0}});
    m_graph.addConnection({{NodeID(1), 1}, {NodeID(3), 1}});
    // RNP -> RNZ
    m_graph.addConnection({{NodeID(3), 0}, {NodeID(4), 0}});
    m_graph.addConnection({{NodeID(3), 1}, {NodeID(4), 1}});
    // RNZ -> R1
    m_graph.addConnection({{NodeID(4), 0}, {NodeID(5), 0}});
    m_graph.addConnection({{NodeID(4), 1}, {NodeID(5), 1}});
    // R1 -> R2
    m_graph.addConnection({{NodeID(5), 0}, {NodeID(6), 0}});
    m_graph.addConnection({{NodeID(5), 1}, {NodeID(6), 1}});
    // R2 -> R3
    m_graph.addConnection({{NodeID(6), 0}, {NodeID(7), 0}});
    m_graph.addConnection({{NodeID(6), 1}, {NodeID(7), 1}});
    // R3 -> R4
    m_graph.addConnection({{NodeID(7), 0}, {NodeID(8), 0}});
    m_graph.addConnection({{NodeID(7), 1}, {NodeID(8), 1}});
    // R4 -> R5
    m_graph.addConnection({{NodeID(8), 0}, {NodeID(9), 0}});
    m_graph.addConnection({{NodeID(8), 1}, {NodeID(9), 1}});
    // R5 -> Output
    m_graph.addConnection({{NodeID(9), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(9), 1}, {NodeID(2), 1}});
}


void CascadeVocal::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_graph.processBlock(buffer, midiMessages);
}
