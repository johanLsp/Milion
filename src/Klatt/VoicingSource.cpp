#include "VoicingSource.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using NodeID =  AudioProcessorGraph::NodeID;

void VoicingSource::setFrequency(int frequency) {
    m_impulseGenerator->setFrequency(frequency);
}

void VoicingSource::prepareToPlay(double sampleRate, int samplesPerBlock) {
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

    m_impulseGenerator = new ImpulseGenerator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_impulseGenerator), NodeID(3));
    m_RGP = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_RGP), NodeID(4));
    m_RGZ = new AntiResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_RGZ), NodeID(5));
    m_RGS = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_RGS), NodeID(6));
    m_AV = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_AV), NodeID(7));
    m_AVS = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_AVS), NodeID(8));

    m_impulseGenerator->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_RGP->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_RGZ->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_RGS->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AV->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AVS->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_RGP->setCenterFrequency(0);
    m_RGP->setBandwidth(100);

    m_RGS->setCenterFrequency(0);
    m_RGS->setBandwidth(200);

    m_RGZ->setCenterFrequency(1500);
    m_RGZ->setBandwidth(6000);

    //  20dB
    m_AV->setGain(1);
    m_AVS->setGain(1);

    // Input -> ImpulseGenerator
    m_graph.addConnection({{NodeID(1), 0}, {NodeID(3), 0}});
    m_graph.addConnection({{NodeID(1), 1}, {NodeID(3), 1}});
    // ImpulseGenerator -> RGP
    m_graph.addConnection({{NodeID(3), 0}, {NodeID(4), 0}});
    m_graph.addConnection({{NodeID(3), 1}, {NodeID(4), 1}});
    // RGP -> RGZ
    m_graph.addConnection({{NodeID(4), 0}, {NodeID(5), 0}});
    m_graph.addConnection({{NodeID(4), 1}, {NodeID(5), 1}});
    // RGP -> RGS
    m_graph.addConnection({{NodeID(4), 0}, {NodeID(6), 0}});
    m_graph.addConnection({{NodeID(4), 1}, {NodeID(6), 1}});
    // RGZ -> AV
    m_graph.addConnection({{NodeID(5), 0}, {NodeID(7), 0}});
    m_graph.addConnection({{NodeID(5), 1}, {NodeID(7), 1}});
    // RGS -> AVS
    m_graph.addConnection({{NodeID(6), 0}, {NodeID(8), 0}});
    m_graph.addConnection({{NodeID(6), 1}, {NodeID(8), 1}});
    // AV -> Output
    m_graph.addConnection({{NodeID(7), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(7), 1}, {NodeID(2), 1}});
    // AVS -> Output
    m_graph.addConnection({{NodeID(8), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(8), 1}, {NodeID(2), 1}});
}

void VoicingSource::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_graph.processBlock(buffer, midiMessages);
}
