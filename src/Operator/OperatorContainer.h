#ifndef MILION_MILIONOPERATORCONTAINER_H_
#define MILION_MILIONOPERATORCONTAINER_H_

#include "JuceHeader.h"
#include "FMOperator.h"

enum class Operator {FM};

class OperatorContainer : public AudioProcessor {
 public:
    OperatorContainer();
    ~OperatorContainer();

    const String getName() const override;
    void releaseResources() override;
    double getTailLengthSeconds() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity);

    void setValueTreeState(AudioProcessorValueTreeState* vst);
    void setOperator(Operator op);
 private:
    AudioProcessorValueTreeState* m_valueTreeState;
    BaseOperator* m_pOperator;
};
#endif  // MILION_MILIONOPERATORCONTAINER_H_
