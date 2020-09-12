#ifndef MILION_MILIONPROCESSOR_H_
#define MILION_MILIONPROCESSOR_H_

#include "Klatt/VoicingSource.h"
#include "Klatt/NoiseSource.h"
#include "Klatt/CascadeVocal.h"
#include "Klatt/ParallelVocal.h"
#include "Klatt/GainProcessor.h"
#include "Klatt/DifferenceProcessor.h"
// This should be included at least once somewhere in the project
#include "JuceHeader.h"

#define NUM_OPERATOR 4
typedef Array<AudioProcessorValueTreeState*> ValueTreeStates;

class MilionProcessor  : public AudioProcessor, 
                              public MidiKeyboardStateListener,
                              public AudioProcessorValueTreeState::Listener {
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
    void parameterChanged (const String& parameterID, float newValue) override;

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
    VoicingSource* m_voicingSource;
    NoiseSource* m_noiseSource;
    CascadeVocal* m_cascadeVocal;
    ParallelVocal* m_parallelVocal;
    GainProcessor* m_AH;
    GainProcessor* m_AF;
    DifferenceProcessor* m_DIFF;

    ValueTreeStates m_valueTreeStates;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionProcessor)
};

#endif  // MILION_MILIONPROCESSOR_H_
