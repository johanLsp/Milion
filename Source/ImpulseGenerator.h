#ifndef MILION_MILIONIMPULSEGENERATOR_H_
#define MILION_MILIONIMPULSEGENERATOR_H_

#include "../JuceLibraryCode/JuceHeader.h"


class ImpulseGenerator {
 public:
    ImpulseGenerator(double sampleRate);
    ~ImpulseGenerator();

    void processBlock(AudioSampleBuffer& buffer);

 private:
    double m_sampleRate;
    double m_frequency;
    uint64_t m_currentTick;
};

#endif  // MILION_MILIONIMPULSEGENERATOR_H_
