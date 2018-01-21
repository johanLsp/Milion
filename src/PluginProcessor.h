#ifndef MILION_MILIONAUDIOPROCESSOR_H_
#define MILION_MILIONAUDIOPROCESSOR_H_

// This should be included at least once somewhere in the project
#include "JuceHeader.h"
#include "Operator/FMOperator.h"
#include "Operator/OperatorContainer.h"
#include <fstream>
#include <sstream>

#define NUM_OPERATOR 8
typedef Array<AudioProcessorValueTreeState*> ValueTreeStates;

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

    AudioProcessorGraph m_graph;

    OperatorContainer* m_operators[8];
    ValueTreeStates m_valueTreeStates;
    std::ofstream file;
};

#endif  // MILION_MILIONAUDIOPROCESSOR_H_
