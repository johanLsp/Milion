#ifndef MILION_MILIONRANDOMGENERATOR_H_
#define MILION_MILIONRANDOMGENERATOR_H_

#include "BaseAudioProcessor.h"

class RandomGenerator : public BaseAudioProcessor {
 public:

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Random Generator Processor"; }
    void setFrequency(int frequency);

 private:
    int m_frequency = 1000;
    uint64_t m_currentTick = 0;
    Random m_random;
};

#endif  // MILION_MILIONRANDOMGENERATOR_H_
