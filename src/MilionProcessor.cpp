#include "MilionProcessor.h"

#include <memory>

#include "MilionEditor.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using NodeID =  AudioProcessorGraph::NodeID;

MilionProcessor::MilionProcessor()
    : AudioProcessor(BusesProperties()
                        .withOutput("Output", AudioChannelSet::stereo(), true)
                       ) {
}

MilionProcessor::~MilionProcessor() {
    m_graph.clear();
}

void MilionProcessor::parameterChanged (const String& parameterID, float newValue) {
    // TODO : getBlockSize() is not the max block size we want here !
    // It just happens to be the same value, but shouldn't be relied upon
    //prepareToPlay(getSampleRate(), getBlockSize());
    suspendProcessing(true);
    // Update parameters - nothing to do here.
    suspendProcessing(false);
}

void MilionProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_graph.setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);

    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

 // Add input and output nodes.
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode), NodeID(1));
    m_graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode), NodeID(2));

    // Add the different stages of the Klatt synthesizer.
    m_voicingSource = new VoicingSource();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_voicingSource), NodeID(3));

    m_noiseSource = new NoiseSource();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_noiseSource), NodeID(4));

    m_cascadeVocal = new CascadeVocal();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_cascadeVocal), NodeID(5));

    m_parallelVocal = new ParallelVocal();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_parallelVocal), NodeID(6));

    m_AH = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_AH), NodeID(7));

    m_AF = new GainProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_AF), NodeID(8));

    m_DIFF = new DifferenceProcessor();
    m_graph.addNode(std::unique_ptr<AudioProcessor>(m_DIFF), NodeID(9));


    m_voicingSource->setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_noiseSource->setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_cascadeVocal->setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_parallelVocal->setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AH->setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AF->setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_DIFF->setPlayConfigDetails(
        getTotalNumOutputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    // Connect stages together. Connect channel 0 & 1 (stereo).
    // Input -> VoicingSource
    m_graph.addConnection({{NodeID(1), 0}, {NodeID(3), 0}});
    m_graph.addConnection({{NodeID(1), 1}, {NodeID(3), 1}});
    // Input -> NoiseSource
    m_graph.addConnection({{NodeID(1), 0}, {NodeID(4), 0}});
    m_graph.addConnection({{NodeID(1), 1}, {NodeID(4), 1}});

    // NoiseSource -> AH
    m_graph.addConnection({{NodeID(4), 0}, {NodeID(7), 0}});
    m_graph.addConnection({{NodeID(4), 1}, {NodeID(7), 1}});
    // NoiseSource -> AF
    m_graph.addConnection({{NodeID(4), 0}, {NodeID(8), 0}});
    m_graph.addConnection({{NodeID(4), 1}, {NodeID(8), 1}});

    // AH -> CascadeVocal
    //m_graph.addConnection({{NodeID(7), 0}, {NodeID(5), 0}});
    //m_graph.addConnection({{NodeID(7), 1}, {NodeID(5), 1}});
    // AF -> ParallelVocal 2-3
    m_graph.addConnection({{NodeID(8), 0}, {NodeID(6), 2}});
    m_graph.addConnection({{NodeID(8), 1}, {NodeID(6), 3}});

    // CascadeVocal -> Output
    //m_graph.addConnection({{NodeID(5), 0}, {NodeID(2), 0}});
    //m_graph.addConnection({{NodeID(5), 1}, {NodeID(2), 1}});

    // ParallelVocal -> Output
    //m_graph.addConnection({{NodeID(6), 0}, {NodeID(2), 0}});
    //m_graph.addConnection({{NodeID(6), 1}, {NodeID(2), 1}});

    // VoicingSource -> CascadeVocal
    //m_graph.addConnection({{NodeID(3), 0}, {NodeID(5), 0}});
    //m_graph.addConnection({{NodeID(3), 1}, {NodeID(5), 1}});
    // VoicingSource -> ParallelVocal 0-1
    //m_graph.addConnection({{NodeID(3), 0}, {NodeID(6), 0}});
    //m_graph.addConnection({{NodeID(3), 1}, {NodeID(6), 1}});

    // VoicingSource -> Output
    m_graph.addConnection({{NodeID(3), 0}, {NodeID(2), 0}});
    m_graph.addConnection({{NodeID(3), 1}, {NodeID(2), 1}});
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
  m_voicingSource->setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

void MilionProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
}
