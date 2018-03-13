#ifndef MILION_MILIONFORMANTOPERATOR_H_
#define MILION_MILIONFORMANTOPERATOR_H_

#define _USE_MATH_DEFINES
#include <cmath>

#include "JuceHeader.h"
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
    ~FormantOperator();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) override;

    const String getName() const override {return "Formant Operator";}

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
