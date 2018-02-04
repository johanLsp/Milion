#ifndef MILION_MILIONFORMANTOPERATOR_H_
#define MILION_MILIONFORMANTOPERATOR_H_

#include "BaseOperator.h"
#include "EnvelopeGenerator.h"
#include "../Utils/Wavetable.h"
/*
    Implements a Frequency Modulation operator
    This is actually done using phase modulation
*/

class FormantOperator : public BaseOperator {

 public:
    FormantOperator(AudioProcessorValueTreeState* vst);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) override;


 private:
    AudioProcessorValueTreeState* m_valueTreeState;
    EnvelopeGenerator m_envelope;
    Wavetable m_wavetable[2];
    double m_frequency;
    double m_mainPhasor[2];
    double m_secondaryPhasor[2];
    double m_bandwidthPhasor[2];
    Wavetable m_window[2];
};

#endif  // MILION_MILIONFORMANTOPERATOR_H_