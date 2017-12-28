#ifndef MILION_MILIONCASCADEVOCAL_H_
#define MILION_MILIONCASCADEVOCAL_H_

#include "DigitalResonator.h"
#include "AntiResonator.h"
#include "../JuceLibraryCode/JuceHeader.h"

class CascadeVocal : public AudioProcessor {
 public:
    CascadeVocal();
    ~CascadeVocal();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
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

 private:
    AudioProcessorGraph m_graph;

    DigitalResonator m_RNP;
    AntiResonator m_RNZ;
    DigitalResonator m_R1;
    DigitalResonator m_R2;
    DigitalResonator m_R3;
    DigitalResonator m_R4;
    DigitalResonator m_R5;
};

#endif  // MILION_MILIONCASCADEVOCAL_H_
