#ifndef MILION_MILIONOPERATORCONTAINER_H_
#define MILION_MILIONOPERATORCONTAINER_H_

#include "JuceHeader.h"
#include "FMOperator.h"
#include "FormantOperator.h"


class OperatorContainer : public AudioProcessor {
 public:
    enum Operator {FM = 0, Formant};

    OperatorContainer();
    ~OperatorContainer();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity);
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity);
    
    void setValueTreeState(AudioProcessorValueTreeState* vst);
    void setOperator(Operator op);
    void releaseResources() override;
    AudioProcessorEditor* createEditor() override {return nullptr;}

    // Configuration
    bool hasEditor() const override {return false;}
    const String getName() const override {return "Operator Container";}
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
    BaseOperator* m_operators[2];
    BaseOperator* m_pCurrentOperator;
};
#endif  // MILION_MILIONOPERATORCONTAINER_H_
