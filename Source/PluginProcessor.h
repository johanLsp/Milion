#ifndef MILION_MILIONAUDIOPROCESSOR_H_
#define MILION_MILIONAUDIOPROCESSOR_H_

#include "../JuceLibraryCode/JuceHeader.h"
#include "VoicingSource.h"
#include <fstream>

class MilionAudioProcessor  : public AudioProcessor, 
                              public MidiKeyboardStateListener {
 public:
    MilionAudioProcessor();
    ~MilionAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    void setFrequency(float frequency);
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

 private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionAudioProcessor)

 private:
    float m_frequency;
    float m_currentPhase;
    float m_phaseIncrement;
    VoicingSource m_voicingSource;
    std::ofstream file;
};

#endif  // MILION_MILIONAUDIOPROCESSOR_H_
