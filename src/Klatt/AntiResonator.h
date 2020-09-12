#ifndef MILION_MILIONANTIRESONATOR_H_
#define MILION_MILIONANTIRESONATOR_H_

#include "BaseAudioProcessor.h"

class AntiResonator : public BaseAudioProcessor {
 public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Anti Resonator Processor"; }

    void setCenterFrequency(double frequency);
    void setBandwidth(double bandwidth);

 private:
    double m_centerFrequency = 1000;
    double m_bandwidth = 100;

    double m_A = 1;
    double m_B = 0;
    double m_C = 0;
    double m_x[2] = {0, 0};
    double m_xx[2] = {0, 0};
};
#endif  // MILION_MILIONANTIRESONATOR_H
