#include "CascadeVocal.h"

CascadeVocal::CascadeVocal() {
}

CascadeVocal::~CascadeVocal() {
}

const String CascadeVocal::getName() const {
    return "Voicing Source Processor";
}

void CascadeVocal::releaseResources() {
}

double CascadeVocal::getTailLengthSeconds() const {
    return 0.0;
}

bool CascadeVocal::acceptsMidi() const {
    return false;
}

bool CascadeVocal::producesMidi() const {
    return false;
}

AudioProcessorEditor* CascadeVocal::createEditor() {
}

bool CascadeVocal::hasEditor() const {
    return false;
}

int CascadeVocal::getNumPrograms() {
    return 1;
}

int CascadeVocal::getCurrentProgram() {
    return 0;
}

void CascadeVocal::setCurrentProgram(int index) {
}

const String CascadeVocal::getProgramName(int index) {
    return {};
}

void CascadeVocal::changeProgramName(int index, const String& newName) {
}

void CascadeVocal::getStateInformation(MemoryBlock& destData) {
}

void CascadeVocal::setStateInformation(const void* data, int sizeInBytes) {
}

void CascadeVocal::prepareToPlay(double sampleRate, int samplesPerBlock) {

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

    m_RNP.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_RNP.setCenterFrequency(250);
    m_RNP.setBandwidth(100);

    m_RNZ.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_RNZ.setCenterFrequency(250);
    m_RNZ.setBandwidth(100);

    m_R1.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R1.setCenterFrequency(450);
    m_R1.setBandwidth(50);

    m_R2.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R2.setCenterFrequency(1450);
    m_R2.setBandwidth(70);

    m_R3.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R3.setCenterFrequency(2450);
    m_R3.setBandwidth(110);

    m_R4.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R4.setCenterFrequency(3300);
    m_R4.setBandwidth(250);

    m_R5.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R5.setCenterFrequency(3750);
    m_R5.setBandwidth(200);

    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);
    m_graph.addNode(&m_RNP, 3);
    m_graph.addNode(&m_RNZ, 4);
    m_graph.addNode(&m_R1, 5);
    m_graph.addNode(&m_R2, 6);
    m_graph.addNode(&m_R3, 7);
    m_graph.addNode(&m_R4, 8);
    m_graph.addNode(&m_R5, 9);

    // Input -> RNP
    m_graph.addConnection(1, 0, 3, 0);
    m_graph.addConnection(1, 1, 3, 1);
    // RNP -> RNZ
    m_graph.addConnection(3, 0, 4, 0);
    m_graph.addConnection(3, 1, 4, 1);
    // RNZ -> R1
    m_graph.addConnection(4, 0, 5, 0);
    m_graph.addConnection(4, 1, 5, 1);
    // R1 -> R2
    m_graph.addConnection(5, 0, 6, 0);
    m_graph.addConnection(5, 1, 6, 1);
    // R2 -> R3
    m_graph.addConnection(6, 0, 7, 0);
    m_graph.addConnection(6, 1, 7, 1);
    // R3 -> R4
    m_graph.addConnection(7, 0, 8, 0);
    m_graph.addConnection(7, 1, 8, 1);
    // R4 -> R5
    m_graph.addConnection(8, 0, 9, 0);
    m_graph.addConnection(8, 1, 9, 1);
    // R5 -> Output
    m_graph.addConnection(9, 0, 2, 0);
    m_graph.addConnection(9, 1, 2, 1);
}


void CascadeVocal::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_graph.processBlock(buffer, midiMessages);
}
