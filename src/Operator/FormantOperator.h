#ifndef MILION_MILIONFORMANTOPERATOR_H_
#define MILION_MILIONFORMANTOPERATOR_H_

#include "BaseOperator.h"
#include "Klatt/ResonantFilter.h"

/*
    Implements a Formant operator
*/

namespace {
constexpr double kPhonemeE[4] = {844, 1656, 2437, 3704};
constexpr double kPhonemeA[4] = {768, 1333, 2522, 3867};
constexpr double kPhonemeI[4] = {324, 2985, 3329, 3807};
constexpr double kPhonemeU[4] = {378, 997, 2343, 3357};

/*
struct Formant {
    double f;
    double bw;
};

constexpr Formant kPhonemes[15][3] = { { {300, 52}, {2045, 200}, {2960, 400} },
                                      { {435, 50}, {1700, 100}, {2585, 140} },
                                      { {405, 62}, {1870, 100}, {2560, 200} },
                                      { {575, 60}, {1605,  90}, {2515, 200} },
                                      { {635, 70}, {1575, 125}, {2450, 320} },
                                      { {700, 130}, {1220, 70}, {2600, 160} },
                                      { {615, 90}, {1015, 100}, {2585, 80} },
                                      { {620, 80}, {1220, 50}, {2550, 140} },
                                      { {495, 80}, {1000, 70}, {2300, 70} },
                                      { {475, 80}, {1140, 100}, {2370, 80} },
                                      { {335, 65}, {1075, 110}, {2200, 140} },
                                      { {445, 100}, {1290, 60}, {1540, 110} },
                                      { {530, 85}, {1540, 85}, {2500, 200} },
                                      { {530, 80}, {1085, 70}, {2450, 110} },
                                      { {455, 70}, {1390, 50}, {2425, 145} } };

*/
}

class FormantOperator : public BaseOperator {

 public:
    FormantOperator();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {};
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {};

 private:
    AudioProcessorGraph m_graph;
    ResonantFilter* m_filters[4];
};

#endif  // MILION_MILIONFORMANTOPERATOR_H_
