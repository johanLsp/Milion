#ifndef MILION_MILIONIMPULSEGENERATOR_H_
#define MILION_MILIONIMPULSEGENERATOR_H_

#include "BaseAudioProcessor.h"

class ImpulseGenerator : public BaseAudioProcessor {
 public:
    const String getName() const override { return "Impulse Generator Processor"; }
    void setFrequency(int frequency) { m_frequency = frequency; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {}
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;

 private:
    int m_frequency = 1000;
    uint64_t m_currentTick = 0;
};

#endif  // MILION_MILIONIMPULSEGENERATOR_H_
