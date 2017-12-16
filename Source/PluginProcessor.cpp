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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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

    m_voicingSource.processBlock(buffer);
    /*
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        m_currentPhase += m_phaseIncrement;
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            float* channelData = buffer.getWritePointer(channel);

            channelData[sample] = sin(m_currentPhase);
        }
    }

*/
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
