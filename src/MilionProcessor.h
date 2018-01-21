#ifndef MILION_MILIONPROCESSOR_H_
#define MILION_MILIONPROCESSOR_H_

// This should be included at least once somewhere in the project
#include "JuceHeader.h"
#include "Operator/FMOperator.h"
#include "Operator/OperatorContainer.h"

#define NUM_OPERATOR 8
typedef Array<AudioProcessorValueTreeState*> ValueTreeStates;

class MilionProcessor  : public AudioProcessor, 
                              public MidiKeyboardStateListener {
 public:
    MilionProcessor();
    ~MilionProcessor();

    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
 
    AudioProcessorEditor* createEditor() override;

    // Plugin configuration
    bool hasEditor() const override {return true;}
    const String getName() const override {return JucePlugin_Name;}
    bool acceptsMidi() const override {return true;}
    bool producesMidi() const override {return false;}
    bool isMidiEffect() const override {return false;}
    double getTailLengthSeconds() const override {return 0.0;}
    // Presets
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int index) override {};
    const String getProgramName(int index) override {return {};}
    void changeProgramName(int index, const String& newName) override {}

 private:
    AudioProcessorGraph m_graph;
    OperatorContainer* m_operators[8];
    ValueTreeStates m_valueTreeStates;    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionProcessor)
};

#endif  // MILION_MILIONPROCESSOR_H_
