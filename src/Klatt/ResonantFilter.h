#ifndef MILION_MILIONRESONANTFILTER_H_
#define MILION_MILIONRESONANTFILTER_H_

#include "BaseAudioProcessor.h"

class ResonantFilter : public BaseAudioProcessor {
 public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;

    void setCenterFrequency(double frequency);
    void setBandwidth(double bandwidth);

    const String getName() const override {return "Resonant Filter";}

 private:
    LinearSmoothedValue<double> m_smoothedFrequency = {440};
    double m_centerFrequency = 440;
    double m_bandwidth = 1000;
    double m_coef[3] = {1, 0, 0};
    double m_y[3] = {0, 0, 0};
};

#endif  // MILION_MILIONRESONANTFILTER_H_
