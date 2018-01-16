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
    m_currentPhase = 0;
    file.open("test.dat");
}

MilionAudioProcessor::~MilionAudioProcessor() {
}

void MilionAudioProcessor::setFrequency(float frequency) {
    m_frequency = frequency;
    m_phaseIncrement = 2 * M_PI * frequency / getSampleRate();
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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
    m_graph.prepareToPlay(sampleRate, samplesPerBlock);

    AudioProcessorGraph::AudioGraphIOProcessor* input =

        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);

    AudioProcessorGraph::AudioGraphIOProcessor* output =
        new AudioProcessorGraph::AudioGraphIOProcessor(
            AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);

    m_FMOP1.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);
    m_FMOP2.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);
    m_graph.addNode(&m_FMOP1, 3);
    m_graph.addNode(&m_FMOP2, 4);

    m_FMOP1.setFrequencyMultiplier(2);
    m_FMOP2.setFrequencyMultiplier(3);

    m_graph.addConnection(1, 0, 3, 0);
    m_graph.addConnection(1, 1, 3, 1);

    m_graph.addConnection(3, 0, 4, 0);
    m_graph.addConnection(3, 1, 4, 1);

    m_graph.addConnection(4, 0, 2, 0);
    m_graph.addConnection(4, 1, 2, 1);
}

void MilionAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.

    m_graph.processBlock(buffer, midiMessages);
    const float* channelData = buffer.getReadPointer(0);


    MilionAudioProcessorEditor* editor = 
        reinterpret_cast<MilionAudioProcessorEditor*>(getActiveEditor());

        if (editor) editor->pushBuffer(channelData, buffer.getNumSamples());
}

//==============================================================================
bool MilionAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MilionAudioProcessor::createEditor() {
    return new MilionAudioProcessorEditor (*this);
}

//==============================================================================
void MilionAudioProcessor::getStateInformation(MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MilionAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new MilionAudioProcessor();
}



void MilionAudioProcessor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
    m_FMOP1.handleNoteOn(midiChannel, midiNoteNumber, velocity);
    m_FMOP2.handleNoteOn(midiChannel, midiNoteNumber, velocity);

    //m_voicingSource.setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    //m_impulseGenerator.setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    //m_randomGenerator.setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    //m_R1.setCenterFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));

    //processor->noteOn(midiChannel, midiNoteNumber, velocity);
    //MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
    //m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    //sendToOutputs (m);
}

//==============================================================================
void MilionAudioProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //MidiMessage m (MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
    //m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    //sendToOutputs (m);
}

