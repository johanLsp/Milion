#include "MilionProcessor.h"
#include "MilionEditor.h"

MilionProcessor::MilionProcessor()
    : AudioProcessor(BusesProperties()
                        .withOutput("Output", AudioChannelSet::stereo(), true)
                       ) {

    AudioProcessorGraph::AudioGraphIOProcessor* output =
        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    m_graph.addNode(output, 1);

    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i] = new OperatorContainer();
        AudioProcessorValueTreeState* vst = new AudioProcessorValueTreeState(*m_operators[i], nullptr);
        m_valueTreeStates.add(vst);
        m_operators[i]->setValueTreeState(vst);
        vst->state = ValueTree(Identifier("Milion" + i));

        m_graph.addNode(m_operators[i], i+2);
    }
}

MilionProcessor::~MilionProcessor() {
    m_graph.clear();
    for (int i = 0; i < NUM_OPERATOR; i++) {
        delete m_valueTreeStates[i];
    }
}

void MilionProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_graph.setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);


    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i]->setOperator(Operator::FM);
        m_operators[i]->setPlayConfigDetails(
            getTotalNumOutputChannels(),
            getTotalNumOutputChannels(),
            sampleRate,
            samplesPerBlock);
    }
    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

    for (int i = m_graph.getNumConnections() - 1; i >= 0; i--)
        m_graph.removeConnection(i);

    for (int i = 0; i < NUM_OPERATOR-1; i++)
        m_graph.addConnection(2+i, 0, 3+i, 0);
    m_graph.addConnection(NUM_OPERATOR+1, 0, 1, 0);
}

void MilionProcessor::releaseResources() {
}

bool MilionProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    return true;
}

void MilionProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    ScopedNoDenormals noDenormals;
    //buffer.clear();
    for (int i = 0; i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    m_graph.processBlock(buffer, midiMessages);

    MilionEditor* editor = 
        reinterpret_cast<MilionEditor*>(getActiveEditor());

    const float* channelData = buffer.getReadPointer(0);
    if (editor) editor->pushBuffer(channelData, buffer.getNumSamples());
}

AudioProcessorEditor* MilionProcessor::createEditor() {
    return new MilionEditor (*this, m_valueTreeStates);
}

void MilionProcessor::getStateInformation(MemoryBlock& destData) {
}

void MilionProcessor::setStateInformation(const void* data, int sizeInBytes) {
}

MilionProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new MilionProcessor();
}

void MilionProcessor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i]->handleNoteOn(midiChannel, midiNoteNumber, velocity);
    }
}

void MilionProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i]->handleNoteOff(midiChannel, midiNoteNumber, velocity);
    }
}
