#ifndef MILION_MILIONNOISESOURCE_H_
#define MILION_MILIONNOISESOURCE_H_

#include "RandomGenerator.h"
#include "DifferenceProcessor.h"
#include "../JuceLibraryCode/JuceHeader.h"

class NoiseSource : public AudioProcessor {
 public:
    NoiseSource();
    ~NoiseSource();
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

    void setFrequency(int frequency);

 private:
    AudioProcessorGraph m_graph;

    RandomGenerator m_randomGenerator;
    DifferenceProcessor m_LPF;
};

#endif  // MILION_MILIONNOISESOURCE_H_
