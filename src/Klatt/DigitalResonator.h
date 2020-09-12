#ifndef MILION_MILIONDIGITALRESONATOR_H_
#define MILION_MILIONDIGITALRESONATOR_H_

#include "BaseAudioProcessor.h"

/* Digital filter - output is given by :
 * y(n) = A*x(n) + B*y(n-1) + C*y(n-2) 
 */

class DigitalResonator : public BaseAudioProcessor {
 public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Digital Resonator Processor"; }

    void setCenterFrequency(double frequency);
    void setBandwidth(double bandwidth);

 private:
    double m_centerFrequency = 1000;
    double m_bandwidth = 100;

    double m_A = 1;
    double m_B = 0;
    double m_C = 0;
    double m_y[2] = {0, 0};
    double m_yy[2] = {0, 0};
};

#endif  // MILION_MILIONDIGITALRESONATOR_H
