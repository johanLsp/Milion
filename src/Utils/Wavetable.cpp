#include "Wavetable.h"


Wavetable::Wavetable(int length)
:m_length(length) {
    m_wavetable = new double[length];
    for (int i = 0; i < length; i++) {
        m_wavetable[i] = sin(2*M_PI*i/length);
    }
}

double Wavetable::operator()(int index) {
    return m_wavetable[index%m_length];
}

Wavetable::~Wavetable() {
    delete[] m_wavetable;
}
