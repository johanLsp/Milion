#ifndef MILION_KLATT_BASEAUDIOPROCSESOR_H_
#define MILION_KLATT_BASEAUDIOPROCSESOR_H_

#include "JuceHeader.h"

class BaseAudioProcessor : public AudioProcessor {
 public:
    BaseAudioProcessor() = default;

    void releaseResources() override {}
    double getTailLengthSeconds() const override { return 0.0; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const String getProgramName(int index) override { return "Default"; }
    void changeProgramName(int index, const String& newName) override {}

    void getStateInformation(MemoryBlock& destData) override {}
    void setStateInformation(const void* data, int sizeInBytes) override {}

    void setCenterFrequency(double frequency);
    void setBandwidth(double bandwidth);
};
#endif  // MILION_KLATT_BASEAUDIOPROCSESOR_H_
