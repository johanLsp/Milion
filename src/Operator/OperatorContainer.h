#ifndef MILION_MILIONOPERATORCONTAINER_H_
#define MILION_MILIONOPERATORCONTAINER_H_

#include "JuceHeader.h"
#include "FMOperator.h"

enum class Operator {FM};

class OperatorContainer : public AudioProcessor {
 public:
    OperatorContainer();
    ~OperatorContainer();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity);
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity);
    
    void setValueTreeState(AudioProcessorValueTreeState* vst);
    void setOperator(Operator op);
    void releaseResources() override;
    AudioProcessorEditor* createEditor() override;

    // Configuration
    bool hasEditor() const override {return false;}
    const String getName() const override {return "FM Operator";}
    double getTailLengthSeconds() const override {return 0.0;}
    bool acceptsMidi() const override {return true;}
    bool producesMidi() const override {return false;}
    // Presets
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int index) override {}
    const String getProgramName(int index) override {return {};}
    void changeProgramName(int index, const String& newName) override {}

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

 private:
    AudioProcessorValueTreeState* m_valueTreeState;
    BaseOperator* m_pOperator;
};
#endif  // MILION_MILIONOPERATORCONTAINER_H_
