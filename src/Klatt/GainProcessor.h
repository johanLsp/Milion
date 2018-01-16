#ifndef MILION_MILIONGAINPROCESSOR_H_
#define MILION_MILIONGAINPROCESSOR_H_

#include <cmath>

class GainProcessor : public AudioProcessor {
 public:
    GainProcessor();
    ~GainProcessor();

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

    void setGain(double gain);

 private:
    double m_gain;
};

#endif  // MILION_MILIONGAINPROCESSOR_H
