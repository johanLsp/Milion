#include "NoiseSource.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using NodeID =  AudioProcessorGraph::NodeID;

NoiseSource::NoiseSource() {
}

NoiseSource::~NoiseSource() {
}

const String NoiseSource::getName() const {
    return "Voicing Source Processor";
}

void NoiseSource::releaseResources() {
}

double NoiseSource::getTailLengthSeconds() const {
    return 0.0;
}

bool NoiseSource::acceptsMidi() const {
    return false;
}

bool NoiseSource::producesMidi() const {
    return false;
}

AudioProcessorEditor* NoiseSource::createEditor() {
}

bool NoiseSource::hasEditor() const {
    return false;
}

int NoiseSource::getNumPrograms() {
    return 1;
}

int NoiseSource::getCurrentProgram() {
    return 0;
}

void NoiseSource::setCurrentProgram(int index) {
}

const String NoiseSource::getProgramName(int index) {
    return {};
}

void NoiseSource::changeProgramName(int index, const String& newName) {
}

void NoiseSource::getStateInformation(MemoryBlock& destData) {
}

void NoiseSource::setStateInformation(const void* data, int sizeInBytes) {
}

void NoiseSource::setFrequency(int frequency) {
    m_randomGenerator->setFrequency(frequency);
}


void NoiseSource::prepareToPlay(double sampleRate, int samplesPerBlock) {

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

    m_randomGenerator = new RandomGenerator();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_randomGenerator), NodeID(3));

    m_LPF = new DifferenceProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_LPF), NodeID(4));

    m_randomGenerator->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_LPF->setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_LPF->setType(DifferenceProcessor::Type::LOWPASS);

    // Input -> RandomGenerator
    m_graph.addConnection({{NodeID(1), 0}, {NodeID(3), 0}});
    m_graph.addConnection({{NodeID(1), 1}, {NodeID(3), 1}});
    // ImpulseGenerator -> LPF
    m_graph.addConnection({{NodeID(3), 0}, {NodeID(4), 0}});
    m_graph.addConnection({{NodeID(3), 1}, {NodeID(4), 1}});
    // LPF -> Output
    m_graph.addConnection({{NodeID(4), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(4), 1}, {NodeID(2), 1}});
}


void NoiseSource::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_graph.processBlock(buffer, midiMessages);
}
