#include "PluginProcessor.h"
#include "PluginEditor.h"

MilionAudioProcessor::MilionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                       .withInput("Input",  AudioChannelSet::stereo(), true)
#endif
                       .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
                       )
#endif
{
    AudioProcessorGraph::AudioGraphIOProcessor* input =

        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);

    AudioProcessorGraph::AudioGraphIOProcessor* output =
        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);

    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i] = new OperatorContainer();
        AudioProcessorValueTreeState* vst = new AudioProcessorValueTreeState(*m_operators[i], nullptr);
        m_valueTreeStates.add(vst);
        m_operators[i]->setValueTreeState(vst);
        m_graph.addNode(m_operators[i], i+3);
    }
}

MilionAudioProcessor::~MilionAudioProcessor() {
    m_graph.clear();
    for (int i = 0; i < NUM_OPERATOR; i++) {
        delete m_valueTreeStates[i];
    }
}

const String MilionAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool MilionAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MilionAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MilionAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MilionAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int MilionAudioProcessor::getNumPrograms() {
    return 1;
}

int MilionAudioProcessor::getCurrentProgram() {
    return 0;
}

void MilionAudioProcessor::setCurrentProgram(int index) {
}

const String MilionAudioProcessor::getProgramName(int index) {
    return {};
}

void MilionAudioProcessor::changeProgramName(int index, const String& newName) {
}

//==============================================================================
void MilionAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    m_graph.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_graph.setProcessingPrecision(AudioProcessor::singlePrecision);


    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i]->setOperator(Operator::FM);
        m_operators[i]->setPlayConfigDetails(
            getTotalNumInputChannels(),
            getTotalNumOutputChannels(),
            sampleRate,
            samplesPerBlock);
    }

    for (int i = m_graph.getNumConnections() - 1; i >= 0; i--)
        m_graph.removeConnection(i);

    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

    m_graph.addConnection(1, 0, 3, 0);
    m_graph.addConnection(1, 1, 3, 1);

    m_graph.addConnection(3, 0, 4, 0);
    m_graph.addConnection(3, 1, 4, 1);

    m_graph.addConnection(4, 0, 2, 0);
    m_graph.addConnection(4, 1, 2, 1);


}

void MilionAudioProcessor::releaseResources() {
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MilionAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void MilionAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    ScopedNoDenormals noDenormals;
    m_graph.processBlock(buffer, midiMessages);

    MilionAudioProcessorEditor* editor = 
        reinterpret_cast<MilionAudioProcessorEditor*>(getActiveEditor());

    const float* channelData = buffer.getReadPointer(0);
    if (editor) editor->pushBuffer(channelData, buffer.getNumSamples());
}

//==============================================================================
bool MilionAudioProcessor::hasEditor() const {
    return true;
}

AudioProcessorEditor* MilionAudioProcessor::createEditor() {
    return new MilionAudioProcessorEditor (*this, m_valueTreeStates);
}

//==============================================================================
void MilionAudioProcessor::getStateInformation(MemoryBlock& destData) {
}

void MilionAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new MilionAudioProcessor();
}

void MilionAudioProcessor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_operators[i]->handleNoteOn(midiChannel, midiNoteNumber, velocity);
    }
}

//==============================================================================
void MilionAudioProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
}

