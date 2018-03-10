#ifndef MILION_MILIONBASEOPERATOR_H_
#define MILION_MILIONBASEOPERATOR_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include "JuceHeader.h"

class BaseOperator : public AudioProcessor {
 public:
    BaseOperator();
    virtual ~BaseOperator() {}

    void releaseResources() override {};
    AudioProcessorEditor* createEditor() override {return nullptr;}
    virtual void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) = 0;
    virtual void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) = 0;
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) = 0;
    virtual void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) = 0;

    // Configuration
    bool hasEditor() const override {return false;}
    const String getName() const override {return "Base Operator";}
    double getTailLengthSeconds() const override {return 0.0;}
    bool acceptsMidi() const override {return true;}
    bool producesMidi() const override {return false;}
    // Presets
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int index) override {}
    const String getProgramName(int index) override {return {};}
    void changeProgramName(int index, const String& newName) override {}

    void getStateInformation(MemoryBlock& destData) override {};
    void setStateInformation(const void* data, int sizeInBytes) override {};
};
#endif  // MILION_MILIONBASEOPERATOR_H_
