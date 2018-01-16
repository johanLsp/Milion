#include "NoiseSource.h"

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
    m_randomGenerator.setFrequency(frequency);
}


void NoiseSource::prepareToPlay(double sampleRate, int samplesPerBlock) {

    m_graph.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);
    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

    AudioProcessorGraph::AudioGraphIOProcessor* input =
        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);

    AudioProcessorGraph::AudioGraphIOProcessor* output =
        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);

    m_randomGenerator.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_LPF.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_LPF.setType(DifferenceProcessor::Type::LOWPASS);




    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);
    m_graph.addNode(&m_randomGenerator, 3);
    m_graph.addNode(&m_LPF, 4);

    // Input -> RandomGenerator
    m_graph.addConnection(1, 0, 3, 0);
    m_graph.addConnection(1, 1, 3, 1);
    // ImpulseGenerator -> LPF
    m_graph.addConnection(3, 0, 4, 0);
    m_graph.addConnection(3, 1, 4, 1);
    // LPF -> Output
    m_graph.addConnection(4, 0, 2, 0);
    m_graph.addConnection(4, 1, 2, 1);
}


void NoiseSource::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_graph.processBlock(buffer, midiMessages);
}
