#ifndef MILION_MILIONAUDIOPROCESSOR_H_
#define MILION_MILIONAUDIOPROCESSOR_H_

// This should be included at least once somewhere in the project
#include "JuceHeader.h"
#include "Klatt/VoicingSource.h"
#include "Klatt/NoiseSource.h"
#include "Klatt/CascadeVocal.h"
#include "Klatt/ParallelVocal.h"
#include "Klatt/GainProcessor.h"
#include "Klatt/DifferenceProcessor.h"
#include "Operator/FMOperator.h"
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

    AudioProcessorGraph m_graph;
/*
    VoicingSource m_voicingSource;
    NoiseSource m_noiseSource;
    CascadeVocal m_cascadeVocal;
    ParallelVocal m_parallelVocal;
    GainProcessor m_AH;
    GainProcessor m_AF;
    DifferenceProcessor m_DIFF;



    ImpulseGenerator m_impulseGenerator;
    RandomGenerator m_randomGenerator;
    DigitalResonator m_R1;
    GainProcessor m_A1;
    DigitalResonator m_R2;
    GainProcessor m_A2;
    DigitalResonator m_R3;
    GainProcessor m_A3;
    DigitalResonator m_R4;
    GainProcessor m_A4;
*/
    FMOperator* m_pFMOP1;
    FMOperator* m_pFMOP2;
    AudioProcessorValueTreeState m_parameters;
    std::ofstream file;
};

#endif  // MILION_MILIONAUDIOPROCESSOR_H_
