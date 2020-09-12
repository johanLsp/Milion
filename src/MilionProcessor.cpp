#include "MilionProcessor.h"

#include <memory>

#include "MilionEditor.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using NodeID =  AudioProcessorGraph::NodeID;

MilionProcessor::MilionProcessor()
    : AudioProcessor(BusesProperties()
                        .withOutput("Output", AudioChannelSet::stereo(), true)
                       ) {
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode), NodeID(1));

    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i] = new OperatorContainer();
        AudioProcessorValueTreeState* vst = new AudioProcessorValueTreeState(
            *m_operators[i], nullptr, "PARAMETERS",
            { std::make_unique<AudioParameterFloat> (
                  "operator_type", "Operator Type", NormalisableRange<float> (0, 1, 1), 0) });
        m_operators[i]->setValueTreeState(vst);
        vst->addParameterListener("operator_type", this);
        vst->state = ValueTree(Identifier("Milion" + std::to_string(i)));
        m_valueTreeStates.add(vst);
        m_graph.addNode(std::unique_ptr<AudioProcessor>(m_operators[i]), NodeID(i+2));
    }
}

MilionProcessor::~MilionProcessor() {
    m_graph.clear();
    for (int i = 0; i < NUM_OPERATOR; i++) {
        delete m_valueTreeStates[i];
    }
}

void MilionProcessor::parameterChanged (const String& parameterID, float newValue) {
    // TODO : getBlockSize() is not the max block size we want here !
    // It just happens to be the same value, but shouldn't be relied upon
    //prepareToPlay(getSampleRate(), getBlockSize());
    suspendProcessing(true);
    for (int i = 0; i < NUM_OPERATOR; i++) {
        std::atomic<float>* type =
            m_valueTreeStates[i]->getRawParameterValue("operator_type");
        if (!type) return;
        switch (static_cast<int>(type->load())) {
            case 0:
                m_operators[i]->setOperator(OperatorContainer::Operator::FM);
                break;
            case 1:
                m_operators[i]->setOperator(OperatorContainer::Operator::Formant);
                break;
        }
    }
    suspendProcessing(false);
}

void MilionProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_graph.setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);


    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i]->setOperator(OperatorContainer::Operator::FM);
        m_operators[i]->setPlayConfigDetails(
            getTotalNumOutputChannels(),
            getTotalNumOutputChannels(),
            sampleRate,
            samplesPerBlock);
    }
    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

    for (int i = m_graph.getNumNodes() - 1; i >= 0; i--)
        m_graph.disconnectNode(NodeID(i));

    for (unsigned int i = 0; i < NUM_OPERATOR-1; i++)
        m_graph.addConnection({{NodeID(2+i), 0}, {NodeID(3+i), 0}});
    m_graph.addConnection({{NodeID(NUM_OPERATOR+1), 0}, {NodeID(1), 0}});
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

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
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
