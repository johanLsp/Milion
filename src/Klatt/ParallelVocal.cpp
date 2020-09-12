#include "ParallelVocal.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using NodeID =  AudioProcessorGraph::NodeID;

void ParallelVocal::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_graph.setPlayConfigDetails(
        2*getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);
    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

    // Add input and output nodes.
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode), NodeID(1));
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode), NodeID(2));

    m_A1 = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_A1), NodeID(3));
    m_R1 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R1), NodeID(4));

    m_AN = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_AN), NodeID(5));
    m_RNP = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_RNP), NodeID(6));

    m_A2 = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_A2), NodeID(7));
    m_R2 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R2), NodeID(8));

    m_A3 = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_A3), NodeID(9));
    m_R3 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R3), NodeID(10));

    m_A4 = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_A4), NodeID(11));
    m_R4 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R4), NodeID(12));

    m_A5 = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_A5), NodeID(13));
    m_R5 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R5), NodeID(14));

    m_A6 = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_A6), NodeID(15));
    m_R6 = new DigitalResonator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_R6), NodeID(16));

    m_AB = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_AB), NodeID(17));

    m_DIFF = new DifferenceProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_DIFF), NodeID(18));

    m_A1->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A1->setGain(1);
    m_R1->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R1->setCenterFrequency(450);
    m_R1->setBandwidth(50);

    m_AN->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_AN->setGain(1);
    m_RNP->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_RNP->setCenterFrequency(250);
    m_RNP->setBandwidth(100);

    m_A2->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A2->setGain(1);
    m_R2->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R2->setCenterFrequency(1450);
    m_R2->setBandwidth(70);

    m_A3->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A3->setGain(1);
    m_R3->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R3->setCenterFrequency(2450);
    m_R3->setBandwidth(110);

    m_A4->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A4->setGain(1);
    m_R4->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R4->setCenterFrequency(3300);
    m_R4->setBandwidth(250);

    m_A5->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A5->setGain(1);
    m_R5->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R5->setCenterFrequency(3750);
    m_R5->setBandwidth(200);

    m_A6->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A6->setGain(1);
    m_R6->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R6->setCenterFrequency(4900);
    m_R6->setBandwidth(1000);

    m_AB->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_AB->setGain(1);

    m_DIFF->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_DIFF->setType(DifferenceProcessor::Type::HIGHPASS);

    // Input 0-1 -> A1
    m_graph.addConnection({{NodeID(1), 0}, {NodeID(3), 0}});
    m_graph.addConnection({{NodeID(1), 1}, {NodeID(3), 1}});
    // Input 0-1 -> DIFF
    m_graph.addConnection({{NodeID(1), 0}, {NodeID(18), 0}});
    m_graph.addConnection({{NodeID(1), 1}, {NodeID(18), 1}});
    // DIFF -> [AN-A4]
    m_graph.addConnection({{NodeID(18), 0}, {NodeID(5), 0}});
    m_graph.addConnection({{NodeID(18), 1}, {NodeID(5), 1}});
    m_graph.addConnection({{NodeID(18), 0}, {NodeID(7), 0}});
    m_graph.addConnection({{NodeID(18), 1}, {NodeID(7), 1}});
    m_graph.addConnection({{NodeID(18), 0}, {NodeID(9), 0}});
    m_graph.addConnection({{NodeID(18), 1}, {NodeID(9), 1}});
    m_graph.addConnection({{NodeID(18), 0}, {NodeID(11), 0}});
    m_graph.addConnection({{NodeID(18), 1}, {NodeID(11), 1}});
    // Input 2-3 -> [AN-AB]
    m_graph.addConnection({{NodeID(1), 2}, {NodeID(5), 0}});
    m_graph.addConnection({{NodeID(1), 3}, {NodeID(5), 1}});
    m_graph.addConnection({{NodeID(1), 2}, {NodeID(7), 0}});
    m_graph.addConnection({{NodeID(1), 3}, {NodeID(7), 1}});
    m_graph.addConnection({{NodeID(1), 2}, {NodeID(9), 0}});
    m_graph.addConnection({{NodeID(1), 3}, {NodeID(9), 1}});
    m_graph.addConnection({{NodeID(1), 2}, {NodeID(11), 0}});
    m_graph.addConnection({{NodeID(1), 3}, {NodeID(11), 1}});
    m_graph.addConnection({{NodeID(1), 2}, {NodeID(13), 0}});
    m_graph.addConnection({{NodeID(1), 3}, {NodeID(13), 1}});
    m_graph.addConnection({{NodeID(1), 2}, {NodeID(15), 0}});
    m_graph.addConnection({{NodeID(1), 3}, {NodeID(15), 1}});
    m_graph.addConnection({{NodeID(1), 2}, {NodeID(17), 0}});
    m_graph.addConnection({{NodeID(1), 3}, {NodeID(17), 1}});
    // A1 -> R1
    m_graph.addConnection({{NodeID(3), 0}, {NodeID(4), 0}});
    m_graph.addConnection({{NodeID(3), 1}, {NodeID(4), 1}});
    // AN -> RN
    m_graph.addConnection({{NodeID(5), 0}, {NodeID(6), 0}});
    m_graph.addConnection({{NodeID(5), 1}, {NodeID(6), 1}});
    // A2 -> R2
    m_graph.addConnection({{NodeID(7), 0}, {NodeID(8), 0}});
    m_graph.addConnection({{NodeID(7), 1}, {NodeID(8), 1}});
    // A3 -> R3
    m_graph.addConnection({{NodeID(9), 0}, {NodeID(10), 0}});
    m_graph.addConnection({{NodeID(9), 1}, {NodeID(10), 1}});
    // A4 -> R4
    m_graph.addConnection({{NodeID(11), 0}, {NodeID(12), 0}});
    m_graph.addConnection({{NodeID(11), 1}, {NodeID(12), 1}});
    // A5 -> R5
    m_graph.addConnection({{NodeID(13), 0}, {NodeID(14), 0}});
    m_graph.addConnection({{NodeID(13), 1}, {NodeID(14), 1}});
    // A6 -> R6
    m_graph.addConnection({{NodeID(15), 0}, {NodeID(16), 0}});
    m_graph.addConnection({{NodeID(15), 1}, {NodeID(16), 1}});
    // [R1-R6] -> Output
    m_graph.addConnection({{NodeID(4), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(4), 1}, {NodeID(2), 1}});
    m_graph.addConnection({{NodeID(6), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(6), 1}, {NodeID(2), 1}});
    m_graph.addConnection({{NodeID(8), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(8), 1}, {NodeID(2), 1}});
    m_graph.addConnection({{NodeID(10), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(10), 1}, {NodeID(2), 1}});
    m_graph.addConnection({{NodeID(12), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(12), 1}, {NodeID(2), 1}});
    m_graph.addConnection({{NodeID(14), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(14), 1}, {NodeID(2), 1}});
    m_graph.addConnection({{NodeID(16), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(16), 1}, {NodeID(2), 1}});
    // AB -> Output
    m_graph.addConnection({{NodeID(17), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(17), 1}, {NodeID(2), 1}});
}


void ParallelVocal::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_graph.processBlock(buffer, midiMessages);
}
