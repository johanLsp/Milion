#ifndef MILION_MILIONDIFFERENCEPROCESSOR_H_
#define MILION_MILIONDIFFERENCEPROCESSOR_H_

#include "BaseAudioProcessor.h"

class DifferenceProcessor : public BaseAudioProcessor {
 public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Digital Resonator Processor"; }

    enum class Type {LOWPASS, HIGHPASS};
    void setType(Type type);

 private:
    double m_xx[2] = {0, 0};
    int m_sign = 1;
};

#endif  // MILION_MILIONDIFFERENCEPROCESSOR_H
