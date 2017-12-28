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

    m_voicingSource.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_noiseSource.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_cascadeVocal.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_parallelVocal.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AH.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_AF.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_DIFF.setPlayConfigDetails(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels(),
        sampleRate,
        samplesPerBlock);

    m_graph.addNode(input, 1);
    m_graph.addNode(output, 2);
    m_graph.addNode(&m_voicingSource, 3);
    m_graph.addNode(&m_noiseSource, 4);
    m_graph.addNode(&m_cascadeVocal, 5);
    m_graph.addNode(&m_parallelVocal, 6);
    m_graph.addNode(&m_AH, 7);
    m_graph.addNode(&m_AF, 8);
    m_graph.addNode(&m_DIFF, 9);

    // Input -> VoicingSource
    m_graph.addConnection(1, 0, 3, 0);
    m_graph.addConnection(1, 1, 3, 1);
    // Input -> NoiseSource
    m_graph.addConnection(1, 0, 4, 0);
    m_graph.addConnection(1, 1, 4, 1);

    // NoiseSource -> AH
    m_graph.addConnection(4, 0, 7, 0);
    m_graph.addConnection(4, 1, 7, 1);
    // NoiseSource -> AF
    m_graph.addConnection(4, 0, 8, 0);
    m_graph.addConnection(4, 1, 8, 1);

    // AH -> CascadeVocal
    //m_graph.addConnection(7, 0, 5, 0);
    //m_graph.addConnection(7, 1, 5, 1);
    // AF -> ParallelVocal 2-3
    m_graph.addConnection(8, 0, 6, 2);
    m_graph.addConnection(8, 1, 6, 3);

    // CascadeVocal -> Output
    //m_graph.addConnection(5, 0, 2, 0);
    //m_graph.addConnection(5, 1, 2, 1);

    // ParallelVocal -> Output
    //m_graph.addConnection(6, 0, 2, 0);
    //m_graph.addConnection(6, 1, 2, 1);

    // VoicingSource -> CascadeVocal
    //m_graph.addConnection(3, 0, 5, 0);
    //m_graph.addConnection(3, 1, 5, 1);
    // VoicingSource -> ParallelVocal 0-1
    //m_graph.addConnection(3, 0, 6, 0);
    //m_graph.addConnection(3, 1, 6, 1);

    // VoicingSource -> Output
    m_graph.addConnection(3, 0, 2, 0);
    m_graph.addConnection(3, 1, 2, 1);
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

    //for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
    //    file << channelData[sample] << std::endl;
    //}
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
    m_voicingSource.setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
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

