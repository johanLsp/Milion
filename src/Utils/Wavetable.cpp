#include "Wavetable.h"


Wavetable::Wavetable(int length)
:m_length(length) {
    for (int i = 0; i < 8; i++) {
        m_wavetable[i] = new double[length];
    }

    for (int i = 0; i < length/4; i++) {
        m_wavetable[0][i] = sin(2*M_PI*i/length);
        m_wavetable[1][i] = sin(2*M_PI*i/length);
        m_wavetable[2][i] = sin(2*M_PI*i/length);
        m_wavetable[3][i] = sin(2*M_PI*i/length);
        m_wavetable[4][i] = sin(4*M_PI*i/length);
        m_wavetable[5][i] = sin(4*M_PI*i/length);
        m_wavetable[6][i] = 1;
    }
    for (int i = length/4; i < length/2; i++) {
        m_wavetable[0][i] = sin(2*M_PI*i/length);
        m_wavetable[1][i] = sin(2*M_PI*i/length);
        m_wavetable[2][i] = sin(2*M_PI*i/length);
        m_wavetable[3][i] = 0;
        m_wavetable[4][i] = sin(4*M_PI*i/length);
        m_wavetable[5][i] = -sin(4*M_PI*i/length);
        m_wavetable[6][i] = 1;
    }
    for (int i = length/2; i < 3*length/4; i++) {
        m_wavetable[0][i] = sin(2*M_PI*i/length);
        m_wavetable[1][i] = 0;
        m_wavetable[2][i] = -sin(2*M_PI*i/length);
        m_wavetable[3][i] = -sin(2*M_PI*i/length);
        m_wavetable[4][i] = 0;
        m_wavetable[5][i] = 0;
        m_wavetable[6][i] = -1;
    }
    for (int i = 3*length/4; i < length; i++) {
        m_wavetable[0][i] = sin(2*M_PI*i/length);
        m_wavetable[1][i] = 0;
        m_wavetable[2][i] = -sin(2*M_PI*i/length);
        m_wavetable[3][i] = 0;
        m_wavetable[4][i] = 0;
        m_wavetable[5][i] = 0;
        m_wavetable[6][i] = -1;
    }

    m_waveformIndex = 1;
}

void Wavetable::setWavetable(int index) {
    m_waveformIndex = index;
}

double Wavetable::operator()(double position) {
    return m_wavetable[m_waveformIndex][static_cast<int>(position*m_length)%m_length];
}

std::string Wavetable::getName(int index) {
    switch (index) {
        case 0:
            return "Sine";
        case 1:
            return "Even 1";
        case 2:
            return "Odd 1";
        case 3:
            return "Res 2";
        case 4:
            return "Even 2";
        case 5:
            return "Odd 2";
        case 6:
            return "Square";
    }
}

Wavetable::~Wavetable() {
    for (int i = 0; i < 8; i++) {
        delete[] m_wavetable[i];
    }
}
