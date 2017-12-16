#include "DigitalResonator.h"

DigitalResonator::DigitalResonator(double sampleRate) {
    m_sampleRate = sampleRate;
    setCenterFrequency(1000);
    setBandwidth(100);
}

void DigitalResonator::setCenterFrequency(double frequency) {
    m_centerFrequency = frequency;
    m_C = -exp(-2*M_PI * m_bandwidth * m_centerFrequency / m_sampleRate);
    m_B = exp(-M_PI * m_bandwidth / m_sampleRate)
        * cos(2*M_PI * m_centerFrequency / m_sampleRate);
    m_A = 1 - m_B - m_C;
}

void DigitalResonator::setBandwidth(double bandwidth) {
    m_bandwidth = bandwidth;
    m_C = -exp(-2*M_PI * m_bandwidth * m_centerFrequency / m_sampleRate);
    m_B = exp(-M_PI * m_bandwidth / m_sampleRate)
        * cos(2*M_PI * m_centerFrequency / m_sampleRate);
    m_A = 1 - m_B - m_C;
}

double DigitalResonator::tick(double input) {
    double output = m_A * input
                  + m_B * m_y
                  + m_C * m_yy;
    m_yy = m_y;
    m_y = output;
    return output;
}
