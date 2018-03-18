#ifndef MILION_MILIONFORMANTOPERATOR_H_
#define MILION_MILIONFORMANTOPERATOR_H_

#include "BaseOperator.h"
#include "Klatt/ResonantFilter.h"
#include "Utils/Wavetable.h"
#include "Operator/EnvelopeGenerator.h"
/*
    Implements a Frequency Modulation operator
    This is actually done using phase modulation
*/

namespace {
constexpr double kPhonemeE[4] = {844, 1656, 2437, 3704};
constexpr double kPhonemeA[4] = {768, 1333, 2522, 3867};
constexpr double kPhonemeI[4] = {324, 2985, 3329, 3807};
constexpr double kPhonemeU[4] = {378, 997, 2343, 3357};
}

class FormantOperator : public BaseOperator {

 public:
    FormantOperator();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) override;


 private:
    AudioSampleBuffer* m_pBuffer;
    AudioProcessorGraph m_graph;
    Wavetable m_Xwave, m_Ywave;
    EnvelopeGenerator m_envelope;
    Wavetable m_wavetable;
    double m_basePhase;
    double m_modulatedPhase;
    double m_frequency;

    double currentX, currentY;
    double Xinc, Yinc;
    ResonantFilter* m_filters[4];
};

#endif  // MILION_MILIONFORMANTOPERATOR_H_
