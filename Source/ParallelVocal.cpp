#include "ParallelVocal.h"

ParallelVocal::ParallelVocal() {
}

ParallelVocal::~ParallelVocal() {
}

const String ParallelVocal::getName() const {
    return "Voicing Source Processor";
}

void ParallelVocal::releaseResources() {
}

double ParallelVocal::getTailLengthSeconds() const {
    return 0.0;
}

bool ParallelVocal::acceptsMidi() const {
    return false;
}

bool ParallelVocal::producesMidi() const {
    return false;
}

AudioProcessorEditor* ParallelVocal::createEditor() {
}

bool ParallelVocal::hasEditor() const {
    return false;
}

int ParallelVocal::getNumPrograms() {
    return 1;
}

int ParallelVocal::getCurrentProgram() {
    return 0;
}

void ParallelVocal::setCurrentProgram(int index) {
}

const String ParallelVocal::getProgramName(int index) {
    return {};
}

void ParallelVocal::changeProgramName(int index, const String& newName) {
}

void ParallelVocal::getStateInformation(MemoryBlock& destData) {
}

void ParallelVocal::setStateInformation(const void* data, int sizeInBytes) {
}

void ParallelVocal::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_graph.setPlayConfigDetails(
        2*getTotalNumInputChannels(),
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

    m_A1.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A1.setGain(1);
    m_R1.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R1.setCenterFrequency(0);
    m_R1.setBandwidth(100);

    m_AN.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_AN.setGain(1);
    m_RNP.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_RNP.setCenterFrequency(0);
    m_RNP.setBandwidth(100);

    m_A2.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A2.setGain(1);
    m_R2.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R2.setCenterFrequency(0);
    m_R2.setBandwidth(100);

    m_A3.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A3.setGain(1);
    m_R3.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R3.setCenterFrequency(0);
    m_R3.setBandwidth(100);

    m_A4.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A4.setGain(1);
    m_R4.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R4.setCenterFrequency(0);
    m_R4.setBandwidth(100);

    m_A5.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A5.setGain(1);
    m_R5.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R5.setCenterFrequency(0);
    m_R5.setBandwidth(100);

    m_A6.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_A6.setGain(1);
    m_R6.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_R6.setCenterFrequency(0);
    m_R6.setBandwidth(100);

    m_AB.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_AB.setGain(1);

    m_DIFF.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);
    m_graph.addNode(&m_A1, 3);
    m_graph.addNode(&m_R1, 4);
    m_graph.addNode(&m_AN, 5);
    m_graph.addNode(&m_RNP, 6);
    m_graph.addNode(&m_A2, 7);
    m_graph.addNode(&m_R2, 8);
    m_graph.addNode(&m_R3, 9);
    m_graph.addNode(&m_A3, 10);
    m_graph.addNode(&m_A4, 11);
    m_graph.addNode(&m_R4, 12);
    m_graph.addNode(&m_A5, 13);
    m_graph.addNode(&m_R5, 14);
    m_graph.addNode(&m_A6, 15);
    m_graph.addNode(&m_R6, 16);
    m_graph.addNode(&m_AB, 17);
    m_graph.addNode(&m_DIFF, 18);

    // Input 0-1 -> A1
    m_graph.addConnection(1, 0, 3, 0);
    m_graph.addConnection(1, 1, 3, 1);
    // Input 0-1 -> DIFF
    m_graph.addConnection(1, 0, 18, 0);
    m_graph.addConnection(1, 1, 18, 1);
    // DIFF -> [AN-A4]
    m_graph.addConnection(18, 0, 5, 0);
    m_graph.addConnection(18, 1, 5, 1);
    m_graph.addConnection(18, 0, 7, 0);
    m_graph.addConnection(18, 1, 7, 1);
    m_graph.addConnection(18, 0, 9, 0);
    m_graph.addConnection(18, 1, 9, 1);
    m_graph.addConnection(18, 0, 11, 0);
    m_graph.addConnection(18, 1, 11, 1);
    // Input 2-3 -> [AN-AB]
    m_graph.addConnection(1, 2, 5, 0);
    m_graph.addConnection(1, 3, 5, 1);
    m_graph.addConnection(1, 2, 7, 0);
    m_graph.addConnection(1, 3, 7, 1);
    m_graph.addConnection(1, 2, 9, 0);
    m_graph.addConnection(1, 3, 9, 1);
    m_graph.addConnection(1, 2, 11, 0);
    m_graph.addConnection(1, 3, 11, 1);
    m_graph.addConnection(1, 2, 13, 0);
    m_graph.addConnection(1, 3, 13, 1);
    m_graph.addConnection(1, 2, 15, 0);
    m_graph.addConnection(1, 3, 15, 1);
    m_graph.addConnection(1, 2, 17, 0);
    m_graph.addConnection(1, 3, 17, 1);
    // A1 -> R1
    m_graph.addConnection(3, 0, 4, 0);
    m_graph.addConnection(3, 1, 4, 1);
    // AN -> RN
    m_graph.addConnection(5, 0, 6, 0);
    m_graph.addConnection(5, 1, 6, 1);
    // A2 -> R2
    m_graph.addConnection(7, 0, 8, 0);
    m_graph.addConnection(7, 1, 8, 1);
    // A3 -> R3
    m_graph.addConnection(9, 0, 10, 0);
    m_graph.addConnection(9, 1, 10, 1);
    // A4 -> R4
    m_graph.addConnection(11, 0, 12, 0);
    m_graph.addConnection(11, 1, 12, 1);
    // A5 -> R5
    m_graph.addConnection(13, 0, 14, 0);
    m_graph.addConnection(13, 1, 14, 1);
    // A6 -> R6
    m_graph.addConnection(15, 0, 16, 0);
    m_graph.addConnection(15, 1, 16, 1);
    // [R1-R6] -> Output
    m_graph.addConnection(4, 0, 2, 0);
    m_graph.addConnection(4, 1, 2, 1);
    m_graph.addConnection(6, 0, 2, 0);
    m_graph.addConnection(6, 1, 2, 1);
    m_graph.addConnection(8, 0, 2, 0);
    m_graph.addConnection(8, 1, 2, 1);
    m_graph.addConnection(10, 0, 2, 0);
    m_graph.addConnection(10, 1, 2, 1);
    m_graph.addConnection(12, 0, 2, 0);
    m_graph.addConnection(12, 1, 2, 1);
    m_graph.addConnection(14, 0, 2, 0);
    m_graph.addConnection(14, 1, 2, 1);
    m_graph.addConnection(16, 0, 2, 0);
    m_graph.addConnection(16, 1, 2, 1);
    // AB -> Output
    m_graph.addConnection(17, 0, 2, 0);
    m_graph.addConnection(17, 1, 2, 1);
}


void ParallelVocal::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_graph.processBlock(buffer, midiMessages);
    /*
    m_pImpulseGenerator->processBlock(buffer);
    m_pRGP->processBlock(buffer);

    AudioSampleBuffer tmpBuffer(m_RGZBuffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        tmpBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples());

    m_pRGS->processBlock(buffer);
    m_pRGZ->processBlock(tmpBuffer);

    buffer.applyGain(m_AVS);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.addFrom(ch, 0, tmpBuffer, ch, 0, buffer.getNumSamples(), m_AV);
    */
}
