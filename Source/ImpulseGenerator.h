#ifndef MILION_MILIONIMPULSEGENERATOR_H_
#define MILION_MILIONIMPULSEGENERATOR_H_

#include "../JuceLibraryCode/JuceHeader.h"


class ImpulseGenerator {
 public:
    ImpulseGenerator(int sampleRate);
    ~ImpulseGenerator();

    void processBlock(AudioSampleBuffer& buffer);
    void setFrequency(int frequency);

 private:
    int m_sampleRate;
    int m_frequency;
    uint64_t m_currentTick;
};

#endif  // MILION_MILIONIMPULSEGENERATOR_H_
