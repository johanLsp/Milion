#ifndef MILION_MILIONIMPULSEGENERATOR_H_
#define MILION_MILIONIMPULSEGENERATOR_H_

#include "../JuceLibraryCode/JuceHeader.h"


class ImpulseGenerator : public AudioProcessor {
 public:
    ImpulseGenerator();
    ~ImpulseGenerator();

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

    void processBlock(AudioSampleBuffer& buffer);
    void setFrequency(int frequency);

 private:
    int m_frequency;
    uint64_t m_currentTick;
};

#endif  // MILION_MILIONIMPULSEGENERATOR_H_
