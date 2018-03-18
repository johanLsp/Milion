#ifndef MILION_MILIONRESONANTFILTER_H_
#define MILION_MILIONRESONANTFILTER_H_

#include "JuceHeader.h"

#define _USE_MATH_DEFINES
#include <cmath>

class ResonantFilter : public AudioProcessor {
 public:
    ResonantFilter();
    ~ResonantFilter();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;

    void setCenterFrequency(double frequency);
    void setBandwidth(double bandwidth);

    const String getName() const override {return "Resonant Filter";}
    void releaseResources() override {}
    double getTailLengthSeconds() const override {return 0.0;};
    bool acceptsMidi() const override {return false;}
    bool producesMidi() const override {return false;}
    AudioProcessorEditor* createEditor() override {return nullptr;}
    bool hasEditor() const override {return false;}

    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int index) override {};
    const String getProgramName(int index) override {return "";}
    void changeProgramName(int index, const String& newName) override {}

    void getStateInformation(MemoryBlock& destData) override {}
    void setStateInformation(const void* data, int sizeInBytes) override {}



 private:
    LinearSmoothedValue<double> m_smoothedFrequency {440};
    double m_centerFrequency = 440;
    double m_bandwidth = 1000;
    double coef1, coef2, coef3;
    double y0, y1, y2;
};

#endif  // MILION_MILIONRESONANTFILTER_H_
