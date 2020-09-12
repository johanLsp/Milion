#ifndef MILION_MILIONGAINPROCESSOR_H_
#define MILION_MILIONGAINPROCESSOR_H_

#include "BaseAudioProcessor.h"

class GainProcessor : public BaseAudioProcessor {
 public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Gain Processor"; }

    void setGain(double gain);

 private:
    double m_gain = 1.0;
};

#endif  // MILION_MILIONGAINPROCESSOR_H
