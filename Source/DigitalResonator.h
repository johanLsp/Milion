#ifndef MILION_MILIONDIGITALRESONATOR_H_
#define MILION_MILIONDIGITALRESONATOR_H_

#include <cmath>
#include "../JuceLibraryCode/JuceHeader.h"

/* Digital filter - output is given by :
 * y(n) = A*x(n) + B*y(n-1) + C*y(n-2) 
 */

class DigitalResonator {
 public:
    explicit DigitalResonator(double sampleRate);
    ~DigitalResonator();

    double tick(double input);
    void setCenterFrequency(double frequency);
    void setBandwidth(double bandwidth);

    void processBlock(AudioSampleBuffer& buffer);

 private:
    double m_sampleRate;
    double m_centerFrequency;
    double m_bandwidth;

    double m_A;
    double m_B;
    double m_C;
    double m_y, m_yy;
};

#endif  // MILION_MILIONDIGITALRESONATOR_H_
