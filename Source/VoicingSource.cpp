#include "VoicingSource.h"

VoicingSource::VoicingSource() {
    m_RGP.setCenterFrequency(0);
    m_RGP.setBandwidth(100);

    m_RGS.setCenterFrequency(0);
    m_RGS.setBandwidth(200);

    m_RGZ.setCenterFrequency(1500);
    m_RGZ.setBandwidth(6000);

    //  20dB
    m_AV.setGain(20);
    m_AVS.setGain(20);
}

VoicingSource::~VoicingSource() {
}

const String VoicingSource::getName() const {
    return "Voicing Source Processor";
}

void VoicingSource::releaseResources() {
}

double VoicingSource::getTailLengthSeconds() const {
    return 0.0;
}

bool VoicingSource::acceptsMidi() const {
    return false;
}

bool VoicingSource::producesMidi() const {
    return false;
}

AudioProcessorEditor* VoicingSource::createEditor() {
}

bool VoicingSource::hasEditor() const {
    return false;
}

int VoicingSource::getNumPrograms() {
    return 1;
}

int VoicingSource::getCurrentProgram() {
    return 0;
}

void VoicingSource::setCurrentProgram(int index) {
}

const String VoicingSource::getProgramName(int index) {
    return {};
}

void VoicingSource::changeProgramName(int index, const String& newName) {
}

void VoicingSource::getStateInformation(MemoryBlock& destData) {
}

void VoicingSource::setStateInformation(const void* data, int sizeInBytes) {
}

void VoicingSource::setFrequency(int frequency) {
    m_impulseGenerator.setFrequency(frequency);
}


void VoicingSource::prepareToPlay(double sampleRate, int samplesPerBlock) {
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

    m_impulseGenerator.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_RGP.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_RGZ.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_RGS.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AV.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AVS.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);
    m_graph.addNode(&m_impulseGenerator, 3);
    m_graph.addNode(&m_RGP, 4);
    m_graph.addNode(&m_RGZ, 5);
    m_graph.addNode(&m_RGS, 6);
    m_graph.addNode(&m_AV,  7);
    m_graph.addNode(&m_AVS, 8);

    // Input -> ImpulseGenerator
    m_graph.addConnection(1, 0, 3, 0);
    m_graph.addConnection(1, 1, 3, 1);
    // ImpulseGenerator -> RGP
    m_graph.addConnection(3, 0, 4, 0);
    m_graph.addConnection(3, 1, 4, 1);
    // RGP -> RGZ
    m_graph.addConnection(4, 0, 5, 0);
    m_graph.addConnection(4, 1, 5, 1);
    // RGP -> RGS
    m_graph.addConnection(4, 0, 6, 0);
    m_graph.addConnection(4, 1, 6, 1);
    // RGZ -> AV
    m_graph.addConnection(5, 0, 7, 0);
    m_graph.addConnection(5, 1, 7, 1);
    // RGS -> AVS
    m_graph.addConnection(6, 0, 8, 0);
    m_graph.addConnection(6, 1, 8, 1);
    // AV -> Output
    m_graph.addConnection(7, 0, 2, 0);
    m_graph.addConnection(7, 1, 2, 1);
    // AVS -> Output
    m_graph.addConnection(8, 0, 2, 0);
    m_graph.addConnection(8, 1, 2, 1);
}


void VoicingSource::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
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
